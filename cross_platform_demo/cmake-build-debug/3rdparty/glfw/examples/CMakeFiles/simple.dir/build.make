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
CMAKE_SOURCE_DIR = /Users/william/git/demo/graphic_demo/cross_platform_demo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug

# Include any dependencies generated for this target.
include 3rdparty/glfw/examples/CMakeFiles/simple.dir/depend.make

# Include the progress variables for this target.
include 3rdparty/glfw/examples/CMakeFiles/simple.dir/progress.make

# Include the compile flags for this target's objects.
include 3rdparty/glfw/examples/CMakeFiles/simple.dir/flags.make

3rdparty/glfw/examples/simple.app/Contents/Resources/glfw.icns: ../3rdparty/glfw/examples/glfw.icns
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Copying OS X content 3rdparty/glfw/examples/simple.app/Contents/Resources/glfw.icns"
	$(CMAKE_COMMAND) -E copy /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/examples/glfw.icns 3rdparty/glfw/examples/simple.app/Contents/Resources/glfw.icns

3rdparty/glfw/examples/CMakeFiles/simple.dir/simple.c.o: 3rdparty/glfw/examples/CMakeFiles/simple.dir/flags.make
3rdparty/glfw/examples/CMakeFiles/simple.dir/simple.c.o: ../3rdparty/glfw/examples/simple.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object 3rdparty/glfw/examples/CMakeFiles/simple.dir/simple.c.o"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/simple.dir/simple.c.o   -c /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/examples/simple.c

3rdparty/glfw/examples/CMakeFiles/simple.dir/simple.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/simple.dir/simple.c.i"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/examples/simple.c > CMakeFiles/simple.dir/simple.c.i

3rdparty/glfw/examples/CMakeFiles/simple.dir/simple.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/simple.dir/simple.c.s"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/examples/simple.c -o CMakeFiles/simple.dir/simple.c.s

3rdparty/glfw/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.o: 3rdparty/glfw/examples/CMakeFiles/simple.dir/flags.make
3rdparty/glfw/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.o: ../3rdparty/glfw/deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object 3rdparty/glfw/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.o"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/simple.dir/__/deps/glad_gl.c.o   -c /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/deps/glad_gl.c

3rdparty/glfw/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/simple.dir/__/deps/glad_gl.c.i"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/deps/glad_gl.c > CMakeFiles/simple.dir/__/deps/glad_gl.c.i

3rdparty/glfw/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/simple.dir/__/deps/glad_gl.c.s"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/deps/glad_gl.c -o CMakeFiles/simple.dir/__/deps/glad_gl.c.s

# Object files for target simple
simple_OBJECTS = \
"CMakeFiles/simple.dir/simple.c.o" \
"CMakeFiles/simple.dir/__/deps/glad_gl.c.o"

# External object files for target simple
simple_EXTERNAL_OBJECTS =

3rdparty/glfw/examples/simple.app/Contents/MacOS/simple: 3rdparty/glfw/examples/CMakeFiles/simple.dir/simple.c.o
3rdparty/glfw/examples/simple.app/Contents/MacOS/simple: 3rdparty/glfw/examples/CMakeFiles/simple.dir/__/deps/glad_gl.c.o
3rdparty/glfw/examples/simple.app/Contents/MacOS/simple: 3rdparty/glfw/examples/CMakeFiles/simple.dir/build.make
3rdparty/glfw/examples/simple.app/Contents/MacOS/simple: 3rdparty/glfw/src/libglfw3.a
3rdparty/glfw/examples/simple.app/Contents/MacOS/simple: 3rdparty/glfw/examples/CMakeFiles/simple.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable simple.app/Contents/MacOS/simple"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
3rdparty/glfw/examples/CMakeFiles/simple.dir/build: 3rdparty/glfw/examples/simple.app/Contents/MacOS/simple
3rdparty/glfw/examples/CMakeFiles/simple.dir/build: 3rdparty/glfw/examples/simple.app/Contents/Resources/glfw.icns

.PHONY : 3rdparty/glfw/examples/CMakeFiles/simple.dir/build

3rdparty/glfw/examples/CMakeFiles/simple.dir/clean:
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/examples && $(CMAKE_COMMAND) -P CMakeFiles/simple.dir/cmake_clean.cmake
.PHONY : 3rdparty/glfw/examples/CMakeFiles/simple.dir/clean

3rdparty/glfw/examples/CMakeFiles/simple.dir/depend:
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/william/git/demo/graphic_demo/cross_platform_demo /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/examples /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/examples /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/examples/CMakeFiles/simple.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : 3rdparty/glfw/examples/CMakeFiles/simple.dir/depend

