# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/liam/Documents/Project/github/nest_track

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/liam/Documents/Project/github/nest_track

# Include any dependencies generated for this target.
include CMakeFiles/EntranceTracker.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/EntranceTracker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/EntranceTracker.dir/flags.make

CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o: CMakeFiles/EntranceTracker.dir/flags.make
CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o: src/main/Main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/liam/Documents/Project/github/nest_track/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o -c /home/liam/Documents/Project/github/nest_track/src/main/Main.cpp

CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/liam/Documents/Project/github/nest_track/src/main/Main.cpp > CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.i

CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/liam/Documents/Project/github/nest_track/src/main/Main.cpp -o CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.s

CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o.requires:
.PHONY : CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o.requires

CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o.provides: CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o.requires
	$(MAKE) -f CMakeFiles/EntranceTracker.dir/build.make CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o.provides.build
.PHONY : CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o.provides

CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o.provides.build: CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o

CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o: CMakeFiles/EntranceTracker.dir/flags.make
CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o: src/display/VideoPlayer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/liam/Documents/Project/github/nest_track/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o -c /home/liam/Documents/Project/github/nest_track/src/display/VideoPlayer.cpp

CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/liam/Documents/Project/github/nest_track/src/display/VideoPlayer.cpp > CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.i

CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/liam/Documents/Project/github/nest_track/src/display/VideoPlayer.cpp -o CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.s

CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o.requires:
.PHONY : CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o.requires

CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o.provides: CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o.requires
	$(MAKE) -f CMakeFiles/EntranceTracker.dir/build.make CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o.provides.build
.PHONY : CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o.provides

CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o.provides.build: CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o

# Object files for target EntranceTracker
EntranceTracker_OBJECTS = \
"CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o" \
"CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o"

# External object files for target EntranceTracker
EntranceTracker_EXTERNAL_OBJECTS =

EntranceTracker: CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o
EntranceTracker: CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o
EntranceTracker: CMakeFiles/EntranceTracker.dir/build.make
EntranceTracker: /usr/local/lib/libopencv_viz.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_videostab.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_superres.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_stitching.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_shape.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_photo.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_objdetect.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_calib3d.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_features2d.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_ml.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_highgui.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_videoio.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_imgcodecs.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_flann.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_video.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_imgproc.so.3.1.0
EntranceTracker: /usr/local/lib/libopencv_core.so.3.1.0
EntranceTracker: CMakeFiles/EntranceTracker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable EntranceTracker"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EntranceTracker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/EntranceTracker.dir/build: EntranceTracker
.PHONY : CMakeFiles/EntranceTracker.dir/build

CMakeFiles/EntranceTracker.dir/requires: CMakeFiles/EntranceTracker.dir/src/main/Main.cpp.o.requires
CMakeFiles/EntranceTracker.dir/requires: CMakeFiles/EntranceTracker.dir/src/display/VideoPlayer.cpp.o.requires
.PHONY : CMakeFiles/EntranceTracker.dir/requires

CMakeFiles/EntranceTracker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/EntranceTracker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/EntranceTracker.dir/clean

CMakeFiles/EntranceTracker.dir/depend:
	cd /home/liam/Documents/Project/github/nest_track && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/liam/Documents/Project/github/nest_track /home/liam/Documents/Project/github/nest_track /home/liam/Documents/Project/github/nest_track /home/liam/Documents/Project/github/nest_track /home/liam/Documents/Project/github/nest_track/CMakeFiles/EntranceTracker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/EntranceTracker.dir/depend

