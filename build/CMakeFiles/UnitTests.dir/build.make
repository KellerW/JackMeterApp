# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wagnerkeller/projects/jackmeter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wagnerkeller/projects/jackmeter/build

# Include any dependencies generated for this target.
include CMakeFiles/UnitTests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/UnitTests.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/UnitTests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/UnitTests.dir/flags.make

CMakeFiles/UnitTests.dir/tests/test_main.cpp.o: CMakeFiles/UnitTests.dir/flags.make
CMakeFiles/UnitTests.dir/tests/test_main.cpp.o: /home/wagnerkeller/projects/jackmeter/tests/test_main.cpp
CMakeFiles/UnitTests.dir/tests/test_main.cpp.o: CMakeFiles/UnitTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/wagnerkeller/projects/jackmeter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/UnitTests.dir/tests/test_main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/UnitTests.dir/tests/test_main.cpp.o -MF CMakeFiles/UnitTests.dir/tests/test_main.cpp.o.d -o CMakeFiles/UnitTests.dir/tests/test_main.cpp.o -c /home/wagnerkeller/projects/jackmeter/tests/test_main.cpp

CMakeFiles/UnitTests.dir/tests/test_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/UnitTests.dir/tests/test_main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wagnerkeller/projects/jackmeter/tests/test_main.cpp > CMakeFiles/UnitTests.dir/tests/test_main.cpp.i

CMakeFiles/UnitTests.dir/tests/test_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/UnitTests.dir/tests/test_main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wagnerkeller/projects/jackmeter/tests/test_main.cpp -o CMakeFiles/UnitTests.dir/tests/test_main.cpp.s

CMakeFiles/UnitTests.dir/tests/test_command.cpp.o: CMakeFiles/UnitTests.dir/flags.make
CMakeFiles/UnitTests.dir/tests/test_command.cpp.o: /home/wagnerkeller/projects/jackmeter/tests/test_command.cpp
CMakeFiles/UnitTests.dir/tests/test_command.cpp.o: CMakeFiles/UnitTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/wagnerkeller/projects/jackmeter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/UnitTests.dir/tests/test_command.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/UnitTests.dir/tests/test_command.cpp.o -MF CMakeFiles/UnitTests.dir/tests/test_command.cpp.o.d -o CMakeFiles/UnitTests.dir/tests/test_command.cpp.o -c /home/wagnerkeller/projects/jackmeter/tests/test_command.cpp

CMakeFiles/UnitTests.dir/tests/test_command.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/UnitTests.dir/tests/test_command.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wagnerkeller/projects/jackmeter/tests/test_command.cpp > CMakeFiles/UnitTests.dir/tests/test_command.cpp.i

CMakeFiles/UnitTests.dir/tests/test_command.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/UnitTests.dir/tests/test_command.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wagnerkeller/projects/jackmeter/tests/test_command.cpp -o CMakeFiles/UnitTests.dir/tests/test_command.cpp.s

CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.o: CMakeFiles/UnitTests.dir/flags.make
CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.o: /home/wagnerkeller/projects/jackmeter/tests/test_simple_peak_processor.cpp
CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.o: CMakeFiles/UnitTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/wagnerkeller/projects/jackmeter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.o -MF CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.o.d -o CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.o -c /home/wagnerkeller/projects/jackmeter/tests/test_simple_peak_processor.cpp

CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wagnerkeller/projects/jackmeter/tests/test_simple_peak_processor.cpp > CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.i

CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wagnerkeller/projects/jackmeter/tests/test_simple_peak_processor.cpp -o CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.s

CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.o: CMakeFiles/UnitTests.dir/flags.make
CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.o: /home/wagnerkeller/projects/jackmeter/tests/test_jackclient.cpp
CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.o: CMakeFiles/UnitTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/wagnerkeller/projects/jackmeter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.o -MF CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.o.d -o CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.o -c /home/wagnerkeller/projects/jackmeter/tests/test_jackclient.cpp

CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wagnerkeller/projects/jackmeter/tests/test_jackclient.cpp > CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.i

CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wagnerkeller/projects/jackmeter/tests/test_jackclient.cpp -o CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.s

CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.o: CMakeFiles/UnitTests.dir/flags.make
CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.o: /home/wagnerkeller/projects/jackmeter/tests/test_FFTProcessor.cpp
CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.o: CMakeFiles/UnitTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/wagnerkeller/projects/jackmeter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.o -MF CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.o.d -o CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.o -c /home/wagnerkeller/projects/jackmeter/tests/test_FFTProcessor.cpp

CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wagnerkeller/projects/jackmeter/tests/test_FFTProcessor.cpp > CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.i

CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wagnerkeller/projects/jackmeter/tests/test_FFTProcessor.cpp -o CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.s

CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.o: CMakeFiles/UnitTests.dir/flags.make
CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.o: /home/wagnerkeller/projects/jackmeter/tests/test_discontinuity_processor.cpp
CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.o: CMakeFiles/UnitTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/wagnerkeller/projects/jackmeter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.o -MF CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.o.d -o CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.o -c /home/wagnerkeller/projects/jackmeter/tests/test_discontinuity_processor.cpp

CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wagnerkeller/projects/jackmeter/tests/test_discontinuity_processor.cpp > CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.i

CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wagnerkeller/projects/jackmeter/tests/test_discontinuity_processor.cpp -o CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.s

# Object files for target UnitTests
UnitTests_OBJECTS = \
"CMakeFiles/UnitTests.dir/tests/test_main.cpp.o" \
"CMakeFiles/UnitTests.dir/tests/test_command.cpp.o" \
"CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.o" \
"CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.o" \
"CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.o" \
"CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.o"

# External object files for target UnitTests
UnitTests_EXTERNAL_OBJECTS =

UnitTests: CMakeFiles/UnitTests.dir/tests/test_main.cpp.o
UnitTests: CMakeFiles/UnitTests.dir/tests/test_command.cpp.o
UnitTests: CMakeFiles/UnitTests.dir/tests/test_simple_peak_processor.cpp.o
UnitTests: CMakeFiles/UnitTests.dir/tests/test_jackclient.cpp.o
UnitTests: CMakeFiles/UnitTests.dir/tests/test_FFTProcessor.cpp.o
UnitTests: CMakeFiles/UnitTests.dir/tests/test_discontinuity_processor.cpp.o
UnitTests: CMakeFiles/UnitTests.dir/build.make
UnitTests: lib/libgtest_main.a
UnitTests: lib/libgmock_main.a
UnitTests: libjackmeter_core.a
UnitTests: /usr/lib/x86_64-linux-gnu/libfmt.so.9.1.0
UnitTests: /usr/lib/x86_64-linux-gnu/libcurses.so
UnitTests: /usr/lib/x86_64-linux-gnu/libform.so
UnitTests: /usr/lib/x86_64-linux-gnu/libgsl.so
UnitTests: /usr/lib/x86_64-linux-gnu/libgslcblas.so
UnitTests: lib/libgmock.a
UnitTests: lib/libgtest.a
UnitTests: /usr/lib/x86_64-linux-gnu/libcurses.so
UnitTests: /usr/lib/x86_64-linux-gnu/libform.so
UnitTests: CMakeFiles/UnitTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/wagnerkeller/projects/jackmeter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable UnitTests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/UnitTests.dir/link.txt --verbose=$(VERBOSE)
	/usr/local/bin/cmake -D TEST_TARGET=UnitTests -D TEST_EXECUTABLE=/home/wagnerkeller/projects/jackmeter/build/UnitTests -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/wagnerkeller/projects/jackmeter/build -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=UnitTests_TESTS -D CTEST_FILE=/home/wagnerkeller/projects/jackmeter/build/UnitTests[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /usr/local/share/cmake-3.29/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
CMakeFiles/UnitTests.dir/build: UnitTests
.PHONY : CMakeFiles/UnitTests.dir/build

CMakeFiles/UnitTests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/UnitTests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/UnitTests.dir/clean

CMakeFiles/UnitTests.dir/depend:
	cd /home/wagnerkeller/projects/jackmeter/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wagnerkeller/projects/jackmeter /home/wagnerkeller/projects/jackmeter /home/wagnerkeller/projects/jackmeter/build /home/wagnerkeller/projects/jackmeter/build /home/wagnerkeller/projects/jackmeter/build/CMakeFiles/UnitTests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/UnitTests.dir/depend

