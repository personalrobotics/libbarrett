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
include programs/CMakeFiles/zerocal.dir/depend.make

# Include the progress variables for this target.
include programs/CMakeFiles/zerocal.dir/progress.make

# Include the compile flags for this target's objects.
include programs/CMakeFiles/zerocal.dir/flags.make

programs/CMakeFiles/zerocal.dir/get_key.cpp.o: programs/CMakeFiles/zerocal.dir/flags.make
programs/CMakeFiles/zerocal.dir/get_key.cpp.o: programs/get_key.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/herb_admin/libbarrett/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object programs/CMakeFiles/zerocal.dir/get_key.cpp.o"
	cd /home/herb_admin/libbarrett/programs && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/zerocal.dir/get_key.cpp.o -c /home/herb_admin/libbarrett/programs/get_key.cpp

programs/CMakeFiles/zerocal.dir/get_key.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zerocal.dir/get_key.cpp.i"
	cd /home/herb_admin/libbarrett/programs && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/herb_admin/libbarrett/programs/get_key.cpp > CMakeFiles/zerocal.dir/get_key.cpp.i

programs/CMakeFiles/zerocal.dir/get_key.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zerocal.dir/get_key.cpp.s"
	cd /home/herb_admin/libbarrett/programs && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/herb_admin/libbarrett/programs/get_key.cpp -o CMakeFiles/zerocal.dir/get_key.cpp.s

programs/CMakeFiles/zerocal.dir/get_key.cpp.o.requires:
.PHONY : programs/CMakeFiles/zerocal.dir/get_key.cpp.o.requires

programs/CMakeFiles/zerocal.dir/get_key.cpp.o.provides: programs/CMakeFiles/zerocal.dir/get_key.cpp.o.requires
	$(MAKE) -f programs/CMakeFiles/zerocal.dir/build.make programs/CMakeFiles/zerocal.dir/get_key.cpp.o.provides.build
.PHONY : programs/CMakeFiles/zerocal.dir/get_key.cpp.o.provides

programs/CMakeFiles/zerocal.dir/get_key.cpp.o.provides.build: programs/CMakeFiles/zerocal.dir/get_key.cpp.o

programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o: programs/CMakeFiles/zerocal.dir/flags.make
programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o: programs/manage_backups.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/herb_admin/libbarrett/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o"
	cd /home/herb_admin/libbarrett/programs && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/zerocal.dir/manage_backups.cpp.o -c /home/herb_admin/libbarrett/programs/manage_backups.cpp

programs/CMakeFiles/zerocal.dir/manage_backups.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zerocal.dir/manage_backups.cpp.i"
	cd /home/herb_admin/libbarrett/programs && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/herb_admin/libbarrett/programs/manage_backups.cpp > CMakeFiles/zerocal.dir/manage_backups.cpp.i

programs/CMakeFiles/zerocal.dir/manage_backups.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zerocal.dir/manage_backups.cpp.s"
	cd /home/herb_admin/libbarrett/programs && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/herb_admin/libbarrett/programs/manage_backups.cpp -o CMakeFiles/zerocal.dir/manage_backups.cpp.s

programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o.requires:
.PHONY : programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o.requires

programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o.provides: programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o.requires
	$(MAKE) -f programs/CMakeFiles/zerocal.dir/build.make programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o.provides.build
.PHONY : programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o.provides

programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o.provides.build: programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o

programs/CMakeFiles/zerocal.dir/zerocal.cpp.o: programs/CMakeFiles/zerocal.dir/flags.make
programs/CMakeFiles/zerocal.dir/zerocal.cpp.o: programs/zerocal.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/herb_admin/libbarrett/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object programs/CMakeFiles/zerocal.dir/zerocal.cpp.o"
	cd /home/herb_admin/libbarrett/programs && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/zerocal.dir/zerocal.cpp.o -c /home/herb_admin/libbarrett/programs/zerocal.cpp

programs/CMakeFiles/zerocal.dir/zerocal.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/zerocal.dir/zerocal.cpp.i"
	cd /home/herb_admin/libbarrett/programs && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/herb_admin/libbarrett/programs/zerocal.cpp > CMakeFiles/zerocal.dir/zerocal.cpp.i

programs/CMakeFiles/zerocal.dir/zerocal.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/zerocal.dir/zerocal.cpp.s"
	cd /home/herb_admin/libbarrett/programs && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/herb_admin/libbarrett/programs/zerocal.cpp -o CMakeFiles/zerocal.dir/zerocal.cpp.s

programs/CMakeFiles/zerocal.dir/zerocal.cpp.o.requires:
.PHONY : programs/CMakeFiles/zerocal.dir/zerocal.cpp.o.requires

programs/CMakeFiles/zerocal.dir/zerocal.cpp.o.provides: programs/CMakeFiles/zerocal.dir/zerocal.cpp.o.requires
	$(MAKE) -f programs/CMakeFiles/zerocal.dir/build.make programs/CMakeFiles/zerocal.dir/zerocal.cpp.o.provides.build
.PHONY : programs/CMakeFiles/zerocal.dir/zerocal.cpp.o.provides

programs/CMakeFiles/zerocal.dir/zerocal.cpp.o.provides.build: programs/CMakeFiles/zerocal.dir/zerocal.cpp.o

# Object files for target zerocal
zerocal_OBJECTS = \
"CMakeFiles/zerocal.dir/get_key.cpp.o" \
"CMakeFiles/zerocal.dir/manage_backups.cpp.o" \
"CMakeFiles/zerocal.dir/zerocal.cpp.o"

# External object files for target zerocal
zerocal_EXTERNAL_OBJECTS =

programs/bt-wam-zerocal: programs/CMakeFiles/zerocal.dir/get_key.cpp.o
programs/bt-wam-zerocal: programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o
programs/bt-wam-zerocal: programs/CMakeFiles/zerocal.dir/zerocal.cpp.o
programs/bt-wam-zerocal: programs/CMakeFiles/zerocal.dir/build.make
programs/bt-wam-zerocal: src/libbarrett.so.1.2.4
programs/bt-wam-zerocal: /usr/lib/x86_64-linux-gnu/libcurses.so
programs/bt-wam-zerocal: /usr/lib/x86_64-linux-gnu/libform.so
programs/bt-wam-zerocal: /usr/lib/x86_64-linux-gnu/libboost_system.so
programs/bt-wam-zerocal: /usr/lib/x86_64-linux-gnu/libboost_thread.so
programs/bt-wam-zerocal: /usr/lib/x86_64-linux-gnu/libboost_python.so
programs/bt-wam-zerocal: /usr/lib/x86_64-linux-gnu/libpthread.so
programs/bt-wam-zerocal: /usr/lib/libxenomai.so
programs/bt-wam-zerocal: /usr/lib/libnative.so
programs/bt-wam-zerocal: /usr/lib/librtdm.so
programs/bt-wam-zerocal: /usr/lib/x86_64-linux-gnu/libpython2.7.so
programs/bt-wam-zerocal: /usr/lib/libxenomai.so
programs/bt-wam-zerocal: /usr/lib/libnative.so
programs/bt-wam-zerocal: programs/CMakeFiles/zerocal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bt-wam-zerocal"
	cd /home/herb_admin/libbarrett/programs && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/zerocal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
programs/CMakeFiles/zerocal.dir/build: programs/bt-wam-zerocal
.PHONY : programs/CMakeFiles/zerocal.dir/build

programs/CMakeFiles/zerocal.dir/requires: programs/CMakeFiles/zerocal.dir/get_key.cpp.o.requires
programs/CMakeFiles/zerocal.dir/requires: programs/CMakeFiles/zerocal.dir/manage_backups.cpp.o.requires
programs/CMakeFiles/zerocal.dir/requires: programs/CMakeFiles/zerocal.dir/zerocal.cpp.o.requires
.PHONY : programs/CMakeFiles/zerocal.dir/requires

programs/CMakeFiles/zerocal.dir/clean:
	cd /home/herb_admin/libbarrett/programs && $(CMAKE_COMMAND) -P CMakeFiles/zerocal.dir/cmake_clean.cmake
.PHONY : programs/CMakeFiles/zerocal.dir/clean

programs/CMakeFiles/zerocal.dir/depend:
	cd /home/herb_admin/libbarrett && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/herb_admin/libbarrett /home/herb_admin/libbarrett/programs /home/herb_admin/libbarrett /home/herb_admin/libbarrett/programs /home/herb_admin/libbarrett/programs/CMakeFiles/zerocal.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : programs/CMakeFiles/zerocal.dir/depend
