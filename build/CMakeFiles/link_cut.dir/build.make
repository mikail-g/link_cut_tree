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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mikaila/Documents/ECE8380

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mikaila/Documents/ECE8380/build

# Include any dependencies generated for this target.
include CMakeFiles/link_cut.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/link_cut.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/link_cut.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/link_cut.dir/flags.make

CMakeFiles/link_cut.dir/link_cut.cpp.o: CMakeFiles/link_cut.dir/flags.make
CMakeFiles/link_cut.dir/link_cut.cpp.o: ../link_cut.cpp
CMakeFiles/link_cut.dir/link_cut.cpp.o: CMakeFiles/link_cut.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mikaila/Documents/ECE8380/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/link_cut.dir/link_cut.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/link_cut.dir/link_cut.cpp.o -MF CMakeFiles/link_cut.dir/link_cut.cpp.o.d -o CMakeFiles/link_cut.dir/link_cut.cpp.o -c /home/mikaila/Documents/ECE8380/link_cut.cpp

CMakeFiles/link_cut.dir/link_cut.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/link_cut.dir/link_cut.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mikaila/Documents/ECE8380/link_cut.cpp > CMakeFiles/link_cut.dir/link_cut.cpp.i

CMakeFiles/link_cut.dir/link_cut.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/link_cut.dir/link_cut.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mikaila/Documents/ECE8380/link_cut.cpp -o CMakeFiles/link_cut.dir/link_cut.cpp.s

# Object files for target link_cut
link_cut_OBJECTS = \
"CMakeFiles/link_cut.dir/link_cut.cpp.o"

# External object files for target link_cut
link_cut_EXTERNAL_OBJECTS =

liblink_cut.a: CMakeFiles/link_cut.dir/link_cut.cpp.o
liblink_cut.a: CMakeFiles/link_cut.dir/build.make
liblink_cut.a: CMakeFiles/link_cut.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mikaila/Documents/ECE8380/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library liblink_cut.a"
	$(CMAKE_COMMAND) -P CMakeFiles/link_cut.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/link_cut.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/link_cut.dir/build: liblink_cut.a
.PHONY : CMakeFiles/link_cut.dir/build

CMakeFiles/link_cut.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/link_cut.dir/cmake_clean.cmake
.PHONY : CMakeFiles/link_cut.dir/clean

CMakeFiles/link_cut.dir/depend:
	cd /home/mikaila/Documents/ECE8380/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mikaila/Documents/ECE8380 /home/mikaila/Documents/ECE8380 /home/mikaila/Documents/ECE8380/build /home/mikaila/Documents/ECE8380/build /home/mikaila/Documents/ECE8380/build/CMakeFiles/link_cut.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/link_cut.dir/depend

