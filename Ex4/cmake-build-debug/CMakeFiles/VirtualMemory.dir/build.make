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
include CMakeFiles/VirtualMemory.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/VirtualMemory.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/VirtualMemory.dir/flags.make

CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.o: CMakeFiles/VirtualMemory.dir/flags.make
CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.o: ../VirtualMemory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.o -c /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/VirtualMemory.cpp

CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/VirtualMemory.cpp > CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.i

CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/VirtualMemory.cpp -o CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.s

CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.o: CMakeFiles/VirtualMemory.dir/flags.make
CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.o: ../PhysicalMemory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.o -c /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/PhysicalMemory.cpp

CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/PhysicalMemory.cpp > CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.i

CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/PhysicalMemory.cpp -o CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.s

# Object files for target VirtualMemory
VirtualMemory_OBJECTS = \
"CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.o" \
"CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.o"

# External object files for target VirtualMemory
VirtualMemory_EXTERNAL_OBJECTS =

libVirtualMemory.a: CMakeFiles/VirtualMemory.dir/VirtualMemory.cpp.o
libVirtualMemory.a: CMakeFiles/VirtualMemory.dir/PhysicalMemory.cpp.o
libVirtualMemory.a: CMakeFiles/VirtualMemory.dir/build.make
libVirtualMemory.a: CMakeFiles/VirtualMemory.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libVirtualMemory.a"
	$(CMAKE_COMMAND) -P CMakeFiles/VirtualMemory.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/VirtualMemory.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/VirtualMemory.dir/build: libVirtualMemory.a

.PHONY : CMakeFiles/VirtualMemory.dir/build

CMakeFiles/VirtualMemory.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/VirtualMemory.dir/cmake_clean.cmake
.PHONY : CMakeFiles/VirtualMemory.dir/clean

CMakeFiles/VirtualMemory.dir/depend:
	cd /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4 /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4 /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug /Users/Muadz/Downloads/HUJI/2020_21_sem_B/OS/Exercises/Ex4/cmake-build-debug/CMakeFiles/VirtualMemory.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/VirtualMemory.dir/depend

