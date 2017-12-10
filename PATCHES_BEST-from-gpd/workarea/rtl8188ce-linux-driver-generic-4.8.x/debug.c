/******************************************************************************
 *
 * Copyright( c ) 2009-2012  Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 *
 * Bug Fixes and enhancements for Linux Kernels >= 3.2
 * by Benjamin Porter <BenjaminPorter86@gmail.com>
 *
 * Project homepage: https://github.com/FreedomBen/rtl8188ce-linux-driver
 *
 *
 * Larry Finger <Larry.Finger@lwfinger.net>
 *****************************************************************************/

#include "wifi.h"

#include <linux/moduleparam.h>

void rtl_dbgp_flag_init( struct ieee80211_hw *hw )
{
	struct rtl_priv *rtlpriv = rtl_priv( hw );
	u8 i;

	rtlpriv->dbg.global_debugcomponents =
	    COMP_ERR | COMP_FW | COMP_INIT | COMP_RECV | COMP_SEND |
	    COMP_MLME | COMP_SCAN | COMP_INTR | COMP_LED | COMP_SEC |
	    COMP_BEACON | COMP_RATE | COMP_RXDESC | COMP_DIG | COMP_TXAGC |
	    COMP_POWER | COMP_POWER_TRACKING | COMP_BB_POWERSAVING | COMP_SWAS |
	    COMP_RF | COMP_TURBO | COMP_RATR | COMP_CMD |
	    COMP_EFUSE | COMP_QOS | COMP_MAC80211 | COMP_REGD | COMP_CHAN |
	    COMP_EASY_CONCURRENT | COMP_EFUSE | COMP_QOS | COMP_MAC80211 |
	    COMP_REGD | COMP_CHAN | COMP_BT_COEXIST;


	for ( i = 0; i < DBGP_TYPE_MAX; i++ )
		rtlpriv->dbg.dbgp_type[i] = 0;

	/*Init Debug flag enable condition */
}
EXPORT_SYMBOL_GPL( rtl_dbgp_flag_init );

#ifdef CONFIG_RTLWIFI_DEBUG
void _rtl_dbg_trace( struct rtl_priv *rtlpriv, int comp, int level,
		    const char *modname, const char *fmt, ... )
{
	if ( unlikely( ( comp & rtlpriv->dbg.global_debugcomponents ) &&
		     ( level <= rtlpriv->dbg.global_debuglevel ) ) ) {
		struct va_format vaf;
		va_list args;

		va_start( args, fmt );

		vaf.fmt = fmt;
		vaf.va = &args;

		printk( KERN_DEBUG "%s:%ps:<%lx-%x> %pV",
		       modname, __builtin_return_address( 0 ),
		       in_interrupt(), in_atomic(),
		       &vaf );

		va_end( args );
	}
}
EXPORT_SYMBOL_GPL( _rtl_dbg_trace );
#endif