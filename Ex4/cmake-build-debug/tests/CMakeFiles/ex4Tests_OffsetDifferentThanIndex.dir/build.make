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
CMAKE_SOURCE_DIR = /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug

# Include any dependencies generated for this target.
include tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/flags.make

tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.o: tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/flags.make
tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.o: ../tests/kb_tests.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.o"
	cd /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/tests && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.o -c /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/tests/kb_tests.cpp

tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.i"
	cd /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/tests/kb_tests.cpp > CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.i

tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.s"
	cd /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/tests/kb_tests.cpp -o CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.s

# Object files for target ex4Tests_OffsetDifferentThanIndex
ex4Tests_OffsetDifferentThanIndex_OBJECTS = \
"CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.o"

# External object files for target ex4Tests_OffsetDifferentThanIndex
ex4Tests_OffsetDifferentThanIndex_EXTERNAL_OBJECTS =

tests/ex4Tests_OffsetDifferentThanIndex: tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/kb_tests.cpp.o
tests/ex4Tests_OffsetDifferentThanIndex: tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/build.make
tests/ex4Tests_OffsetDifferentThanIndex: libOffsetDifferentThanIndexMemory.a
tests/ex4Tests_OffsetDifferentThanIndex: lib/libgtest_maind.a
tests/ex4Tests_OffsetDifferentThanIndex: lib/libgtestd.a
tests/ex4Tests_OffsetDifferentThanIndex: tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ex4Tests_OffsetDifferentThanIndex"
	cd /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/build: tests/ex4Tests_OffsetDifferentThanIndex

.PHONY : tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/build

tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/clean:
	cd /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/tests && $(CMAKE_COMMAND) -P CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/clean

tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/depend:
	cd /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4 /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/tests /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/tests /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/ex4Tests_OffsetDifferentThanIndex.dir/depend

