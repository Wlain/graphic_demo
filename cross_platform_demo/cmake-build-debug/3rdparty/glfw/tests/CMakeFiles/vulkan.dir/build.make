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
include 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/depend.make

# Include the progress variables for this target.
include 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/progress.make

# Include the compile flags for this target's objects.
include 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/flags.make

3rdparty/glfw/tests/CMakeFiles/vulkan.dir/vulkan.c.o: 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/flags.make
3rdparty/glfw/tests/CMakeFiles/vulkan.dir/vulkan.c.o: ../3rdparty/glfw/tests/vulkan.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/vulkan.c.o"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vulkan.dir/vulkan.c.o   -c /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/tests/vulkan.c

3rdparty/glfw/tests/CMakeFiles/vulkan.dir/vulkan.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vulkan.dir/vulkan.c.i"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/tests/vulkan.c > CMakeFiles/vulkan.dir/vulkan.c.i

3rdparty/glfw/tests/CMakeFiles/vulkan.dir/vulkan.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vulkan.dir/vulkan.c.s"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/tests/vulkan.c -o CMakeFiles/vulkan.dir/vulkan.c.s

3rdparty/glfw/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o: 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/flags.make
3rdparty/glfw/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o: ../3rdparty/glfw/deps/glad_vulkan.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o   -c /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/deps/glad_vulkan.c

3rdparty/glfw/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.i"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/deps/glad_vulkan.c > CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.i

3rdparty/glfw/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.s"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/deps/glad_vulkan.c -o CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.s

# Object files for target vulkan
vulkan_OBJECTS = \
"CMakeFiles/vulkan.dir/vulkan.c.o" \
"CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o"

# External object files for target vulkan
vulkan_EXTERNAL_OBJECTS =

3rdparty/glfw/tests/vulkan: 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/vulkan.c.o
3rdparty/glfw/tests/vulkan: 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o
3rdparty/glfw/tests/vulkan: 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/build.make
3rdparty/glfw/tests/vulkan: 3rdparty/glfw/src/libglfw3.a
3rdparty/glfw/tests/vulkan: 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable vulkan"
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vulkan.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
3rdparty/glfw/tests/CMakeFiles/vulkan.dir/build: 3rdparty/glfw/tests/vulkan

.PHONY : 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/build

3rdparty/glfw/tests/CMakeFiles/vulkan.dir/clean:
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/tests && $(CMAKE_COMMAND) -P CMakeFiles/vulkan.dir/cmake_clean.cmake
.PHONY : 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/clean

3rdparty/glfw/tests/CMakeFiles/vulkan.dir/depend:
	cd /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/william/git/demo/graphic_demo/cross_platform_demo /Users/william/git/demo/graphic_demo/cross_platform_demo/3rdparty/glfw/tests /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/tests /Users/william/git/demo/graphic_demo/cross_platform_demo/cmake-build-debug/3rdparty/glfw/tests/CMakeFiles/vulkan.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : 3rdparty/glfw/tests/CMakeFiles/vulkan.dir/depend

