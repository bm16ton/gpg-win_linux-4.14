# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests/build

# Include any dependencies generated for this target.
include CMakeFiles/test-debug.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test-debug.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test-debug.dir/flags.make

CMakeFiles/test-debug.dir/TestMtpDebug.o: CMakeFiles/test-debug.dir/flags.make
CMakeFiles/test-debug.dir/TestMtpDebug.o: ../TestMtpDebug.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test-debug.dir/TestMtpDebug.o"
	/usr/bin/ccache  g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-debug.dir/TestMtpDebug.o -c /home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests/TestMtpDebug.cpp

CMakeFiles/test-debug.dir/TestMtpDebug.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-debug.dir/TestMtpDebug.i"
	/usr/bin/ccache  g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests/TestMtpDebug.cpp > CMakeFiles/test-debug.dir/TestMtpDebug.i

CMakeFiles/test-debug.dir/TestMtpDebug.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-debug.dir/TestMtpDebug.s"
	/usr/bin/ccache  g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests/TestMtpDebug.cpp -o CMakeFiles/test-debug.dir/TestMtpDebug.s

CMakeFiles/test-debug.dir/TestMtpDebug.o.requires:

.PHONY : CMakeFiles/test-debug.dir/TestMtpDebug.o.requires

CMakeFiles/test-debug.dir/TestMtpDebug.o.provides: CMakeFiles/test-debug.dir/TestMtpDebug.o.requires
	$(MAKE) -f CMakeFiles/test-debug.dir/build.make CMakeFiles/test-debug.dir/TestMtpDebug.o.provides.build
.PHONY : CMakeFiles/test-debug.dir/TestMtpDebug.o.provides

CMakeFiles/test-debug.dir/TestMtpDebug.o.provides.build: CMakeFiles/test-debug.dir/TestMtpDebug.o


# Object files for target test-debug
test__debug_OBJECTS = \
"CMakeFiles/test-debug.dir/TestMtpDebug.o"

# External object files for target test-debug
test__debug_EXTERNAL_OBJECTS =

test-debug: CMakeFiles/test-debug.dir/TestMtpDebug.o
test-debug: CMakeFiles/test-debug.dir/build.make
test-debug: CMakeFiles/test-debug.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test-debug"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-debug.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test-debug.dir/build: test-debug

.PHONY : CMakeFiles/test-debug.dir/build

CMakeFiles/test-debug.dir/requires: CMakeFiles/test-debug.dir/TestMtpDebug.o.requires

.PHONY : CMakeFiles/test-debug.dir/requires

CMakeFiles/test-debug.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test-debug.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test-debug.dir/clean

CMakeFiles/test-debug.dir/depend:
	cd /home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests /home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests /home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests/build /home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests/build /home/maddocks/kernels/linux-gpd-4.14.14/tools/16ton/mtp_adb/mtp-server/tests/build/CMakeFiles/test-debug.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test-debug.dir/depend

