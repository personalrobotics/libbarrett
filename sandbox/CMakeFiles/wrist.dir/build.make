# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/herb_admin/libbarrett

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/herb_admin/libbarrett

# Include any dependencies generated for this target.
include sandbox/CMakeFiles/wrist.dir/depend.make

# Include the progress variables for this target.
include sandbox/CMakeFiles/wrist.dir/progress.make

# Include the compile flags for this target's objects.
include sandbox/CMakeFiles/wrist.dir/flags.make

sandbox/CMakeFiles/wrist.dir/wrist.cpp.o: sandbox/CMakeFiles/wrist.dir/flags.make
sandbox/CMakeFiles/wrist.dir/wrist.cpp.o: sandbox/wrist.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/herb_admin/libbarrett/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object sandbox/CMakeFiles/wrist.dir/wrist.cpp.o"
	cd /home/herb_admin/libbarrett/sandbox && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/wrist.dir/wrist.cpp.o -c /home/herb_admin/libbarrett/sandbox/wrist.cpp

sandbox/CMakeFiles/wrist.dir/wrist.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wrist.dir/wrist.cpp.i"
	cd /home/herb_admin/libbarrett/sandbox && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/herb_admin/libbarrett/sandbox/wrist.cpp > CMakeFiles/wrist.dir/wrist.cpp.i

sandbox/CMakeFiles/wrist.dir/wrist.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wrist.dir/wrist.cpp.s"
	cd /home/herb_admin/libbarrett/sandbox && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/herb_admin/libbarrett/sandbox/wrist.cpp -o CMakeFiles/wrist.dir/wrist.cpp.s

sandbox/CMakeFiles/wrist.dir/wrist.cpp.o.requires:
.PHONY : sandbox/CMakeFiles/wrist.dir/wrist.cpp.o.requires

sandbox/CMakeFiles/wrist.dir/wrist.cpp.o.provides: sandbox/CMakeFiles/wrist.dir/wrist.cpp.o.requires
	$(MAKE) -f sandbox/CMakeFiles/wrist.dir/build.make sandbox/CMakeFiles/wrist.dir/wrist.cpp.o.provides.build
.PHONY : sandbox/CMakeFiles/wrist.dir/wrist.cpp.o.provides

sandbox/CMakeFiles/wrist.dir/wrist.cpp.o.provides.build: sandbox/CMakeFiles/wrist.dir/wrist.cpp.o

# Object files for target wrist
wrist_OBJECTS = \
"CMakeFiles/wrist.dir/wrist.cpp.o"

# External object files for target wrist
wrist_EXTERNAL_OBJECTS =

sandbox/wrist: sandbox/CMakeFiles/wrist.dir/wrist.cpp.o
sandbox/wrist: sandbox/CMakeFiles/wrist.dir/build.make
sandbox/wrist: /usr/lib/x86_64-linux-gnu/libboost_system.so
sandbox/wrist: /usr/lib/x86_64-linux-gnu/libboost_thread.so
sandbox/wrist: /usr/lib/x86_64-linux-gnu/libboost_python.so
sandbox/wrist: /usr/lib/x86_64-linux-gnu/libpthread.so
sandbox/wrist: /usr/lib/libnative.so
sandbox/wrist: /usr/lib/libxenomai.so
sandbox/wrist: /usr/lib/librtdm.so
sandbox/wrist: /usr/lib/x86_64-linux-gnu/libpython2.7.so
sandbox/wrist: src/libbarrett.so.1.2.4
sandbox/wrist: /usr/lib/x86_64-linux-gnu/libboost_system.so
sandbox/wrist: /usr/lib/x86_64-linux-gnu/libboost_thread.so
sandbox/wrist: /usr/lib/x86_64-linux-gnu/libboost_python.so
sandbox/wrist: /usr/lib/x86_64-linux-gnu/libpthread.so
sandbox/wrist: /usr/lib/libnative.so
sandbox/wrist: /usr/lib/libxenomai.so
sandbox/wrist: /usr/lib/librtdm.so
sandbox/wrist: /usr/lib/x86_64-linux-gnu/libpython2.7.so
sandbox/wrist: sandbox/CMakeFiles/wrist.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable wrist"
	cd /home/herb_admin/libbarrett/sandbox && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wrist.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sandbox/CMakeFiles/wrist.dir/build: sandbox/wrist
.PHONY : sandbox/CMakeFiles/wrist.dir/build

sandbox/CMakeFiles/wrist.dir/requires: sandbox/CMakeFiles/wrist.dir/wrist.cpp.o.requires
.PHONY : sandbox/CMakeFiles/wrist.dir/requires

sandbox/CMakeFiles/wrist.dir/clean:
	cd /home/herb_admin/libbarrett/sandbox && $(CMAKE_COMMAND) -P CMakeFiles/wrist.dir/cmake_clean.cmake
.PHONY : sandbox/CMakeFiles/wrist.dir/clean

sandbox/CMakeFiles/wrist.dir/depend:
	cd /home/herb_admin/libbarrett && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/herb_admin/libbarrett /home/herb_admin/libbarrett/sandbox /home/herb_admin/libbarrett /home/herb_admin/libbarrett/sandbox /home/herb_admin/libbarrett/sandbox/CMakeFiles/wrist.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sandbox/CMakeFiles/wrist.dir/depend

