# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nadina/Github/whisker-bulllet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nadina/Github/whisker-bulllet

# Include any dependencies generated for this target.
include src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/depend.make

# Include the progress variables for this target.
include src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/progress.make

# Include the compile flags for this target's objects.
include src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/flags.make

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/flags.make
src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o: src/Bullet3Geometry/b3ConvexHullComputer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nadina/Github/whisker-bulllet/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o"
	cd /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o -c /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry/b3ConvexHullComputer.cpp

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.i"
	cd /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry/b3ConvexHullComputer.cpp > CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.i

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.s"
	cd /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry/b3ConvexHullComputer.cpp -o CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.s

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o.requires:

.PHONY : src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o.requires

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o.provides: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o.requires
	$(MAKE) -f src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/build.make src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o.provides.build
.PHONY : src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o.provides

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o.provides.build: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o


src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/flags.make
src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o: src/Bullet3Geometry/b3GeometryUtil.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nadina/Github/whisker-bulllet/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o"
	cd /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o -c /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry/b3GeometryUtil.cpp

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.i"
	cd /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry/b3GeometryUtil.cpp > CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.i

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.s"
	cd /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry/b3GeometryUtil.cpp -o CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.s

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o.requires:

.PHONY : src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o.requires

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o.provides: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o.requires
	$(MAKE) -f src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/build.make src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o.provides.build
.PHONY : src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o.provides

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o.provides.build: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o


# Object files for target Bullet3Geometry
Bullet3Geometry_OBJECTS = \
"CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o" \
"CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o"

# External object files for target Bullet3Geometry
Bullet3Geometry_EXTERNAL_OBJECTS =

lib/libBullet3Geometry.a: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o
lib/libBullet3Geometry.a: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o
lib/libBullet3Geometry.a: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/build.make
lib/libBullet3Geometry.a: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nadina/Github/whisker-bulllet/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../../lib/libBullet3Geometry.a"
	cd /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry && $(CMAKE_COMMAND) -P CMakeFiles/Bullet3Geometry.dir/cmake_clean_target.cmake
	cd /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Bullet3Geometry.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/build: lib/libBullet3Geometry.a

.PHONY : src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/build

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/requires: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o.requires
src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/requires: src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o.requires

.PHONY : src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/requires

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/clean:
	cd /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry && $(CMAKE_COMMAND) -P CMakeFiles/Bullet3Geometry.dir/cmake_clean.cmake
.PHONY : src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/clean

src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/depend:
	cd /home/nadina/Github/whisker-bulllet && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nadina/Github/whisker-bulllet /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry /home/nadina/Github/whisker-bulllet /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry /home/nadina/Github/whisker-bulllet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/depend
