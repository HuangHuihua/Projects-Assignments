# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:/hhh/Documents/CourseFiles/6771/ass2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:/hhh/Documents/CourseFiles/6771/ass2/build

# Include any dependencies generated for this target.
include CMakeFiles/catch2_main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/catch2_main.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/catch2_main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/catch2_main.dir/flags.make

CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.obj: CMakeFiles/catch2_main.dir/flags.make
CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.obj: CMakeFiles/catch2_main.dir/includes_CXX.rsp
CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.obj: D:/hhh/Documents/CourseFiles/6771/ass2/lib/catch2_main.cpp
CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.obj: CMakeFiles/catch2_main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:/hhh/Documents/CourseFiles/6771/ass2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.obj"
	D:/MSYS2/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.obj -MF CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.obj.d -o CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.obj -c D:/hhh/Documents/CourseFiles/6771/ass2/lib/catch2_main.cpp

CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.i"
	D:/MSYS2/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:/hhh/Documents/CourseFiles/6771/ass2/lib/catch2_main.cpp > CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.i

CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.s"
	D:/MSYS2/mingw64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:/hhh/Documents/CourseFiles/6771/ass2/lib/catch2_main.cpp -o CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.s

# Object files for target catch2_main
catch2_main_OBJECTS = \
"CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.obj"

# External object files for target catch2_main
catch2_main_EXTERNAL_OBJECTS =

libcatch2_main.a: CMakeFiles/catch2_main.dir/lib/catch2_main.cpp.obj
libcatch2_main.a: CMakeFiles/catch2_main.dir/build.make
libcatch2_main.a: CMakeFiles/catch2_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:/hhh/Documents/CourseFiles/6771/ass2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libcatch2_main.a"
	$(CMAKE_COMMAND) -P CMakeFiles/catch2_main.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/catch2_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/catch2_main.dir/build: libcatch2_main.a
.PHONY : CMakeFiles/catch2_main.dir/build

CMakeFiles/catch2_main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/catch2_main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/catch2_main.dir/clean

CMakeFiles/catch2_main.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" D:/hhh/Documents/CourseFiles/6771/ass2 D:/hhh/Documents/CourseFiles/6771/ass2 D:/hhh/Documents/CourseFiles/6771/ass2/build D:/hhh/Documents/CourseFiles/6771/ass2/build D:/hhh/Documents/CourseFiles/6771/ass2/build/CMakeFiles/catch2_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/catch2_main.dir/depend

