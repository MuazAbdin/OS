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
include CMakeFiles/draft.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/draft.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/draft.dir/flags.make

CMakeFiles/draft.dir/draft.cpp.o: CMakeFiles/draft.dir/flags.make
CMakeFiles/draft.dir/draft.cpp.o: ../draft.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/draft.dir/draft.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/draft.dir/draft.cpp.o -c /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/draft.cpp

CMakeFiles/draft.dir/draft.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/draft.dir/draft.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/draft.cpp > CMakeFiles/draft.dir/draft.cpp.i

CMakeFiles/draft.dir/draft.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/draft.dir/draft.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/draft.cpp -o CMakeFiles/draft.dir/draft.cpp.s

# Object files for target draft
draft_OBJECTS = \
"CMakeFiles/draft.dir/draft.cpp.o"

# External object files for target draft
draft_EXTERNAL_OBJECTS =

draft: CMakeFiles/draft.dir/draft.cpp.o
draft: CMakeFiles/draft.dir/build.make
draft: CMakeFiles/draft.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable draft"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/draft.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/draft.dir/build: draft

.PHONY : CMakeFiles/draft.dir/build

CMakeFiles/draft.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/draft.dir/cmake_clean.cmake
.PHONY : CMakeFiles/draft.dir/clean

CMakeFiles/draft.dir/depend:
	cd /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3 /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3 /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex3/cmake-build-debug/CMakeFiles/draft.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/draft.dir/depend

