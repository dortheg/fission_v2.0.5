# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.21.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.21.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Applications/fission_v2.0.5/sample_codes/events/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Applications/fission_v2.0.5/sample_codes/events/build

# Include any dependencies generated for this target.
include CMakeFiles/events.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/events.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/events.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/events.dir/flags.make

CMakeFiles/events.dir/events.cpp.o: CMakeFiles/events.dir/flags.make
CMakeFiles/events.dir/events.cpp.o: /Applications/fission_v2.0.5/sample_codes/events/src/events.cpp
CMakeFiles/events.dir/events.cpp.o: CMakeFiles/events.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Applications/fission_v2.0.5/sample_codes/events/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/events.dir/events.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/events.dir/events.cpp.o -MF CMakeFiles/events.dir/events.cpp.o.d -o CMakeFiles/events.dir/events.cpp.o -c /Applications/fission_v2.0.5/sample_codes/events/src/events.cpp

CMakeFiles/events.dir/events.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/events.dir/events.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Applications/fission_v2.0.5/sample_codes/events/src/events.cpp > CMakeFiles/events.dir/events.cpp.i

CMakeFiles/events.dir/events.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/events.dir/events.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Applications/fission_v2.0.5/sample_codes/events/src/events.cpp -o CMakeFiles/events.dir/events.cpp.s

# Object files for target events
events_OBJECTS = \
"CMakeFiles/events.dir/events.cpp.o"

# External object files for target events
events_EXTERNAL_OBJECTS =

events: CMakeFiles/events.dir/events.cpp.o
events: CMakeFiles/events.dir/build.make
events: /Applications/fission_v2.0.5/lib/libFission.dylib
events: CMakeFiles/events.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Applications/fission_v2.0.5/sample_codes/events/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable events"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/events.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/events.dir/build: events
.PHONY : CMakeFiles/events.dir/build

CMakeFiles/events.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/events.dir/cmake_clean.cmake
.PHONY : CMakeFiles/events.dir/clean

CMakeFiles/events.dir/depend:
	cd /Applications/fission_v2.0.5/sample_codes/events/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Applications/fission_v2.0.5/sample_codes/events/src /Applications/fission_v2.0.5/sample_codes/events/src /Applications/fission_v2.0.5/sample_codes/events/build /Applications/fission_v2.0.5/sample_codes/events/build /Applications/fission_v2.0.5/sample_codes/events/build/CMakeFiles/events.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/events.dir/depend

