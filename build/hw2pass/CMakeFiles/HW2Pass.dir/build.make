# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /n/eecs583b/home/marcusvc/cse583/register_alloc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /n/eecs583b/home/marcusvc/cse583/register_alloc/build

# Include any dependencies generated for this target.
include hw2pass/CMakeFiles/HW2Pass.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include hw2pass/CMakeFiles/HW2Pass.dir/compiler_depend.make

# Include the progress variables for this target.
include hw2pass/CMakeFiles/HW2Pass.dir/progress.make

# Include the compile flags for this target's objects.
include hw2pass/CMakeFiles/HW2Pass.dir/flags.make

hw2pass/CMakeFiles/HW2Pass.dir/hw2pass.cpp.o: hw2pass/CMakeFiles/HW2Pass.dir/flags.make
hw2pass/CMakeFiles/HW2Pass.dir/hw2pass.cpp.o: ../hw2pass/hw2pass.cpp
hw2pass/CMakeFiles/HW2Pass.dir/hw2pass.cpp.o: hw2pass/CMakeFiles/HW2Pass.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/n/eecs583b/home/marcusvc/cse583/register_alloc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object hw2pass/CMakeFiles/HW2Pass.dir/hw2pass.cpp.o"
	cd /n/eecs583b/home/marcusvc/cse583/register_alloc/build/hw2pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT hw2pass/CMakeFiles/HW2Pass.dir/hw2pass.cpp.o -MF CMakeFiles/HW2Pass.dir/hw2pass.cpp.o.d -o CMakeFiles/HW2Pass.dir/hw2pass.cpp.o -c /n/eecs583b/home/marcusvc/cse583/register_alloc/hw2pass/hw2pass.cpp

hw2pass/CMakeFiles/HW2Pass.dir/hw2pass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HW2Pass.dir/hw2pass.cpp.i"
	cd /n/eecs583b/home/marcusvc/cse583/register_alloc/build/hw2pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /n/eecs583b/home/marcusvc/cse583/register_alloc/hw2pass/hw2pass.cpp > CMakeFiles/HW2Pass.dir/hw2pass.cpp.i

hw2pass/CMakeFiles/HW2Pass.dir/hw2pass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HW2Pass.dir/hw2pass.cpp.s"
	cd /n/eecs583b/home/marcusvc/cse583/register_alloc/build/hw2pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /n/eecs583b/home/marcusvc/cse583/register_alloc/hw2pass/hw2pass.cpp -o CMakeFiles/HW2Pass.dir/hw2pass.cpp.s

# Object files for target HW2Pass
HW2Pass_OBJECTS = \
"CMakeFiles/HW2Pass.dir/hw2pass.cpp.o"

# External object files for target HW2Pass
HW2Pass_EXTERNAL_OBJECTS =

hw2pass/HW2Pass.so: hw2pass/CMakeFiles/HW2Pass.dir/hw2pass.cpp.o
hw2pass/HW2Pass.so: hw2pass/CMakeFiles/HW2Pass.dir/build.make
hw2pass/HW2Pass.so: hw2pass/CMakeFiles/HW2Pass.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/n/eecs583b/home/marcusvc/cse583/register_alloc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module HW2Pass.so"
	cd /n/eecs583b/home/marcusvc/cse583/register_alloc/build/hw2pass && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HW2Pass.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
hw2pass/CMakeFiles/HW2Pass.dir/build: hw2pass/HW2Pass.so
.PHONY : hw2pass/CMakeFiles/HW2Pass.dir/build

hw2pass/CMakeFiles/HW2Pass.dir/clean:
	cd /n/eecs583b/home/marcusvc/cse583/register_alloc/build/hw2pass && $(CMAKE_COMMAND) -P CMakeFiles/HW2Pass.dir/cmake_clean.cmake
.PHONY : hw2pass/CMakeFiles/HW2Pass.dir/clean

hw2pass/CMakeFiles/HW2Pass.dir/depend:
	cd /n/eecs583b/home/marcusvc/cse583/register_alloc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /n/eecs583b/home/marcusvc/cse583/register_alloc /n/eecs583b/home/marcusvc/cse583/register_alloc/hw2pass /n/eecs583b/home/marcusvc/cse583/register_alloc/build /n/eecs583b/home/marcusvc/cse583/register_alloc/build/hw2pass /n/eecs583b/home/marcusvc/cse583/register_alloc/build/hw2pass/CMakeFiles/HW2Pass.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : hw2pass/CMakeFiles/HW2Pass.dir/depend

