# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/l/cs144/wl-sponge

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/l/cs144/wl-sponge

# Utility rule file for check_lab2.

# Include the progress variables for this target.
include CMakeFiles/check_lab2.dir/progress.make

CMakeFiles/check_lab2:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/l/cs144/wl-sponge/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Testing the TCP receiver..."
	/usr/bin/ctest --output-on-failure --timeout 10 -R 't_recv_|t_wrapping_|t_strm_reassem_|t_byte_stream|_dt'

check_lab2: CMakeFiles/check_lab2
check_lab2: CMakeFiles/check_lab2.dir/build.make

.PHONY : check_lab2

# Rule to build all files generated by this target.
CMakeFiles/check_lab2.dir/build: check_lab2

.PHONY : CMakeFiles/check_lab2.dir/build

CMakeFiles/check_lab2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/check_lab2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/check_lab2.dir/clean

CMakeFiles/check_lab2.dir/depend:
	cd /home/l/cs144/wl-sponge && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/l/cs144/wl-sponge /home/l/cs144/wl-sponge /home/l/cs144/wl-sponge /home/l/cs144/wl-sponge /home/l/cs144/wl-sponge/CMakeFiles/check_lab2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/check_lab2.dir/depend

