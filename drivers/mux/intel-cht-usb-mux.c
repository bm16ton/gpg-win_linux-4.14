/*
 * Intel Cherrytrail USB OTG MUX driver
 *
 * Copyright (c) 2016-2017 Hans de Goede <hdegoede@redhat.com>
 *
 * Loosely based on android x86 kernel code which is:
 *
 * Copyright (C) 2014 Intel Corp.
 *
 * Author: Wu, Hao
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation, or (at your option)
 * any later version.
 */

#include <linux/acpi.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/extcon.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mux/driver.h>
#include <linux/mux/usb.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>

/* register definition */
#define DUAL_ROLE_CFG0			0x68
#define SW_VBUS_VALID			(1 << 24)
#define SW_IDPIN_EN			(1 << 21)
#define SW_IDPIN			(1 << 20)

#define DUAL_ROLE_CFG1			0x6c
#define HOST_MODE			(1 << 29)

#define DUAL_ROLE_CFG1_POLL_TIMEOUT	1000

#define DRV_NAME			"intel_cht_usb_mux"

struct intel_cht_usb_data {
	struct mutex cfg0_lock;
	void __iomem *base;
	struct extcon_dev *vbus_extcon;
	struct notifier_block vbus_nb;
	struct work_struct vbus_work;
};

struct intel_cht_extcon_info {
	const char *hid;
	int hrv;
	const char *extcon;
};

static const struct intel_cht_extcon_info vbus_providers[] = {
	{ .hid = "INT33F4", .hrv = -1, .extcon = "axp288_extcon" },
	{ .hid = "INT34D3", .hrv =  3, .extcon = "cht_wcove_pwrsrc" },
};

static int intel_cht_usb_set_mux(struct mux_control *mux, int state)
{
	struct intel_cht_usb_data *data = mux_chip_priv(mux->chip);
	unsigned long timeout;
	bool host_mode;
	u32 val;

	mutex_lock(&data->cfg0_lock);

	/* Set idpin value as requested */
	val = readl(data->base + DUAL_ROLE_CFG0);
	if (state == MUX_USB_DEVICE) {
		val |= SW_IDPIN;
		host_mode = false;
	} else {
		val &= ~SW_IDPIN;
		host_mode = true;
	}
	val |= SW_IDPIN_EN;
	writel(val, data->base + DUAL_ROLE_CFG0);

	mutex_unlock(&data->cfg0_lock);

	/* In most case it takes about 600ms to finish mode switching */
	timeout = jiffies + msecs_to_jiffies(DUAL_ROLE_CFG1_POLL_TIMEOUT);

	/* Polling on CFG1 register to confirm mode switch.*/
	do {
		val = readl(data->base + DUAL_ROLE_CFG1);
		if (!!(val & HOST_MODE) == host_mode)
			return 0;

		/* Interval for polling is set to about 5 - 10 ms */
		usleep_range(5000, 10000);
	} while (time_before(jiffies, timeout));

	dev_warn(&mux->chip->dev, "Timeout waiting for mux to switch\n");
	return -ETIMEDOUT;
}

static void intel_cht_usb_set_vbus_valid(struct intel_cht_usb_data *data,
					     bool valid)
{
	u32 val;

	mutex_lock(&data->cfg0_lock);

	val = readl(data->base + DUAL_ROLE_CFG0);
	if (valid)
		val |= SW_VBUS_VALID;
	else
		val &= ~SW_VBUS_VALID;

	val |= SW_IDPIN_EN;
	writel(val, data->base + DUAL_ROLE_CFG0);

	mutex_unlock(&data->cfg0_lock);
}

static void intel_cht_usb_vbus_work(struct work_struct *work)
{
	struct intel_cht_usb_data *data =
		container_of(work, struct intel_cht_usb_data, vbus_work);
	const unsigned int vbus_cables[] = {
		EXTCON_CHG_USB_SDP, EXTCON_CHG_USB_CDP, EXTCON_CHG_USB_DCP,
		EXTCON_CHG_USB_ACA, EXTCON_CHG_USB_FAST,
	};
	bool vbus_present = false;
	int i;

	for (i = 0; i < ARRAY_SIZE(vbus_cables); i++) {
		if (extcon_get_state(data->vbus_extcon, vbus_cables[i]) > 0) {
			vbus_present = true;
			break;
		}
	}

	intel_cht_usb_set_vbus_valid(data, vbus_present);
}

static int intel_cht_usb_vbus_extcon_evt(struct notifier_block *nb,
					     unsigned long event, void *param)
{
	struct intel_cht_usb_data *data =
		container_of(nb, struct intel_cht_usb_data, vbus_nb);

	schedule_work(&data->vbus_work);

	return NOTIFY_OK;
}

static const struct mux_control_ops intel_cht_usb_ops = {
	.set = intel_cht_usb_set_mux,
};

static int intel_cht_usb_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct intel_cht_usb_data *data;
	struct mux_chip *mux_chip;
	struct resource *res;
	resource_size_t size;
	int i, ret;

	mux_chip = devm_mux_chip_alloc(dev, 1, sizeof(*data));
	if (IS_ERR(mux_chip))
		return PTR_ERR(mux_chip);

	mux_chip->ops = &intel_cht_usb_ops;
	mux_chip->mux[0].idle_state = MUX_USB_DEVICE;
	/* Keep initial state as is, for e.g. booting from an USB disk */
	mux_chip->mux[0].init_as_is = true;
	mux_chip->mux[0].states = MUX_USB_STATES;
	data = mux_chip_priv(mux_chip);
	mutex_init(&data->cfg0_lock);

	/*
	 * Besides controlling the mux we also need to control the vbus_valid
	 * flag for device/gadget mode to work properly. To do this we monitor
	 * the extcon interface exported by the PMIC drivers for the PMICs used
	 * with the Cherry Trail SoC.
	 *
	 * We try to get the extcon_dev before registering the mux as this
	 * may lead to us exiting with -EPROBE_DEFER.
	 */
	for (i = 0 ; i < ARRAY_SIZE(vbus_providers); i++) {
		if (!acpi_dev_present(vbus_providers[i].hid, NULL,
				      vbus_providers[i].hrv))
			continue;

		data->vbus_extcon = extcon_get_extcon_dev(
						vbus_providers[i].extcon);
		if (data->vbus_extcon == NULL)
			return -EPROBE_DEFER;

		dev_info(dev, "using extcon '%s' for vbus-valid\n",
			 vbus_providers[i].extcon);
		break;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	size = (res->end + 1) - res->start;
	data->base = devm_ioremap_nocache(dev, res->start, size);
	if (IS_ERR(data->base)) {
		ret = PTR_ERR(data->base);
		dev_err(dev, "can't iomap registers: %d\n", ret);
		return ret;
	}

	ret = devm_mux_chip_register(dev, mux_chip);
	if (ret < 0)
		return ret;

	if (data->vbus_extcon) {
		INIT_WORK(&data->vbus_work, intel_cht_usb_vbus_work);
		data->vbus_nb.notifier_call = intel_cht_usb_vbus_extcon_evt;
		ret = devm_extcon_register_notifier_all(dev, data->vbus_extcon,
							&data->vbus_nb);
		if (ret) {
			dev_err(dev, "can't register vbus extcon notifier: %d\n",
				ret);
			return ret;
		}

		/* Sync initial mode */
		schedule_work(&data->vbus_work);
	}

	return 0;
}

static const struct platform_device_id intel_cht_usb_table[] = {
	{ .name = DRV_NAME },
	{},
};
MODULE_DEVICE_TABLE(platform, intel_cht_usb_table);

static struct platform_driver intel_cht_usb_driver = {
	.driver = {
		.name = DRV_NAME,
	},
	.id_table = intel_cht_usb_table,
	.probe = intel_cht_usb_probe,
};

module_platform_driver(intel_cht_usb_driver);

MODULE_AUTHOR("Hans de Goede <hdegoede@redhat.com>");
MODULE_DESCRIPTION("Intel Cherrytrail USB mux driver");
MODULE_LICENSE("GPL");
