# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/SampleClient.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SampleClient.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SampleClient.dir/flags.make

CMakeFiles/SampleClient.dir/SampleClient.cpp.o: CMakeFiles/SampleClient.dir/flags.make
CMakeFiles/SampleClient.dir/SampleClient.cpp.o: ../SampleClient.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SampleClient.dir/SampleClient.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SampleClient.dir/SampleClient.cpp.o -c /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/SampleClient.cpp

CMakeFiles/SampleClient.dir/SampleClient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SampleClient.dir/SampleClient.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/SampleClient.cpp > CMakeFiles/SampleClient.dir/SampleClient.cpp.i

CMakeFiles/SampleClient.dir/SampleClient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SampleClient.dir/SampleClient.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/SampleClient.cpp -o CMakeFiles/SampleClient.dir/SampleClient.cpp.s

# Object files for target SampleClient
SampleClient_OBJECTS = \
"CMakeFiles/SampleClient.dir/SampleClient.cpp.o"

# External object files for target SampleClient
SampleClient_EXTERNAL_OBJECTS =

SampleClient: CMakeFiles/SampleClient.dir/SampleClient.cpp.o
SampleClient: CMakeFiles/SampleClient.dir/build.make
SampleClient: CMakeFiles/SampleClient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SampleClient"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SampleClient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SampleClient.dir/build: SampleClient

.PHONY : CMakeFiles/SampleClient.dir/build

CMakeFiles/SampleClient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SampleClient.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SampleClient.dir/clean

CMakeFiles/SampleClient.dir/depend:
	cd /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3 /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3 /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug/CMakeFiles/SampleClient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SampleClient.dir/depend

