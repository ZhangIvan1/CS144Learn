# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/CS144Learn

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/CS144Learn/build

# Utility rule file for check_webget.

# Include any custom commands dependencies for this target.
include CMakeFiles/check_webget.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/check_webget.dir/progress.make

CMakeFiles/check_webget:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/root/CS144Learn/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Testing webget..."
	/usr/bin/ctest --output-on-failure --timeout 10 -R 't_webget'

check_webget: CMakeFiles/check_webget
check_webget: CMakeFiles/check_webget.dir/build.make
.PHONY : check_webget

# Rule to build all files generated by this target.
CMakeFiles/check_webget.dir/build: check_webget
.PHONY : CMakeFiles/check_webget.dir/build

CMakeFiles/check_webget.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/check_webget.dir/cmake_clean.cmake
.PHONY : CMakeFiles/check_webget.dir/clean

CMakeFiles/check_webget.dir/depend:
	cd /root/CS144Learn/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/CS144Learn /root/CS144Learn /root/CS144Learn/build /root/CS144Learn/build /root/CS144Learn/build/CMakeFiles/check_webget.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/check_webget.dir/depend

