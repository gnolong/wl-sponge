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

# Include any dependencies generated for this target.
include apps/CMakeFiles/webget.dir/depend.make

# Include the progress variables for this target.
include apps/CMakeFiles/webget.dir/progress.make

# Include the compile flags for this target's objects.
include apps/CMakeFiles/webget.dir/flags.make

apps/CMakeFiles/webget.dir/webget.cc.o: apps/CMakeFiles/webget.dir/flags.make
apps/CMakeFiles/webget.dir/webget.cc.o: apps/webget.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/l/cs144/wl-sponge/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/CMakeFiles/webget.dir/webget.cc.o"
	cd /home/l/cs144/wl-sponge/apps && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webget.dir/webget.cc.o -c /home/l/cs144/wl-sponge/apps/webget.cc

apps/CMakeFiles/webget.dir/webget.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webget.dir/webget.cc.i"
	cd /home/l/cs144/wl-sponge/apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/l/cs144/wl-sponge/apps/webget.cc > CMakeFiles/webget.dir/webget.cc.i

apps/CMakeFiles/webget.dir/webget.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webget.dir/webget.cc.s"
	cd /home/l/cs144/wl-sponge/apps && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/l/cs144/wl-sponge/apps/webget.cc -o CMakeFiles/webget.dir/webget.cc.s

# Object files for target webget
webget_OBJECTS = \
"CMakeFiles/webget.dir/webget.cc.o"

# External object files for target webget
webget_EXTERNAL_OBJECTS =

apps/webget: apps/CMakeFiles/webget.dir/webget.cc.o
apps/webget: apps/CMakeFiles/webget.dir/build.make
apps/webget: libsponge/libsponge.a
apps/webget: /usr/lib/x86_64-linux-gnu/libpthread.so
apps/webget: apps/CMakeFiles/webget.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/l/cs144/wl-sponge/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable webget"
	cd /home/l/cs144/wl-sponge/apps && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webget.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/CMakeFiles/webget.dir/build: apps/webget

.PHONY : apps/CMakeFiles/webget.dir/build

apps/CMakeFiles/webget.dir/clean:
	cd /home/l/cs144/wl-sponge/apps && $(CMAKE_COMMAND) -P CMakeFiles/webget.dir/cmake_clean.cmake
.PHONY : apps/CMakeFiles/webget.dir/clean

apps/CMakeFiles/webget.dir/depend:
	cd /home/l/cs144/wl-sponge && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/l/cs144/wl-sponge /home/l/cs144/wl-sponge/apps /home/l/cs144/wl-sponge /home/l/cs144/wl-sponge/apps /home/l/cs144/wl-sponge/apps/CMakeFiles/webget.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/CMakeFiles/webget.dir/depend

