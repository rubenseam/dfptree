# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/speed/rubens/DFPTree/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/speed/rubens/DFPTree/bin

# Include any dependencies generated for this target.
include tree/CMakeFiles/tree.dir/depend.make

# Include the progress variables for this target.
include tree/CMakeFiles/tree.dir/progress.make

# Include the compile flags for this target's objects.
include tree/CMakeFiles/tree.dir/flags.make

tree/CMakeFiles/tree.dir/tree.cpp.o: tree/CMakeFiles/tree.dir/flags.make
tree/CMakeFiles/tree.dir/tree.cpp.o: /home/speed/rubens/DFPTree/src/tree/tree.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/speed/rubens/DFPTree/bin/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tree/CMakeFiles/tree.dir/tree.cpp.o"
	cd /home/speed/rubens/DFPTree/bin/tree && mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tree.dir/tree.cpp.o -c /home/speed/rubens/DFPTree/src/tree/tree.cpp

tree/CMakeFiles/tree.dir/tree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tree.dir/tree.cpp.i"
	cd /home/speed/rubens/DFPTree/bin/tree && mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/speed/rubens/DFPTree/src/tree/tree.cpp > CMakeFiles/tree.dir/tree.cpp.i

tree/CMakeFiles/tree.dir/tree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tree.dir/tree.cpp.s"
	cd /home/speed/rubens/DFPTree/bin/tree && mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/speed/rubens/DFPTree/src/tree/tree.cpp -o CMakeFiles/tree.dir/tree.cpp.s

tree/CMakeFiles/tree.dir/tree.cpp.o.requires:
.PHONY : tree/CMakeFiles/tree.dir/tree.cpp.o.requires

tree/CMakeFiles/tree.dir/tree.cpp.o.provides: tree/CMakeFiles/tree.dir/tree.cpp.o.requires
	$(MAKE) -f tree/CMakeFiles/tree.dir/build.make tree/CMakeFiles/tree.dir/tree.cpp.o.provides.build
.PHONY : tree/CMakeFiles/tree.dir/tree.cpp.o.provides

tree/CMakeFiles/tree.dir/tree.cpp.o.provides.build: tree/CMakeFiles/tree.dir/tree.cpp.o

# Object files for target tree
tree_OBJECTS = \
"CMakeFiles/tree.dir/tree.cpp.o"

# External object files for target tree
tree_EXTERNAL_OBJECTS =

tree/libtree.so: tree/CMakeFiles/tree.dir/tree.cpp.o
tree/libtree.so: tree/CMakeFiles/tree.dir/build.make
tree/libtree.so: tree/CMakeFiles/tree.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library libtree.so"
	cd /home/speed/rubens/DFPTree/bin/tree && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tree.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tree/CMakeFiles/tree.dir/build: tree/libtree.so
.PHONY : tree/CMakeFiles/tree.dir/build

tree/CMakeFiles/tree.dir/requires: tree/CMakeFiles/tree.dir/tree.cpp.o.requires
.PHONY : tree/CMakeFiles/tree.dir/requires

tree/CMakeFiles/tree.dir/clean:
	cd /home/speed/rubens/DFPTree/bin/tree && $(CMAKE_COMMAND) -P CMakeFiles/tree.dir/cmake_clean.cmake
.PHONY : tree/CMakeFiles/tree.dir/clean

tree/CMakeFiles/tree.dir/depend:
	cd /home/speed/rubens/DFPTree/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/speed/rubens/DFPTree/src /home/speed/rubens/DFPTree/src/tree /home/speed/rubens/DFPTree/bin /home/speed/rubens/DFPTree/bin/tree /home/speed/rubens/DFPTree/bin/tree/CMakeFiles/tree.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tree/CMakeFiles/tree.dir/depend

