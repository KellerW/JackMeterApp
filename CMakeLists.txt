cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(JackMeter VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find wxWidgets (use find_package to look for COMPONENTS)
find_package(wxWidgets REQUIRED COMPONENTS core base)

# Include the wxWidgets macros
include(${wxWidgets_USE_FILE})

find_package(GSL REQUIRED)

# Find the fmt library
find_package(fmt REQUIRED)

# Find JACK and SndFile using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(JACK REQUIRED jack)
pkg_check_modules(SNDFILE REQUIRED sndfile)

# Find ncurses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Include directories for JACK and SndFile
include_directories(${JACK_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})

# Link directories for JACK and SndFile
link_directories(${JACK_LIBRARY_DIRS} ${SNDFILE_LIBRARY_DIRS})

# Add the include directories
include_directories(include)

# Create a library from core logic
add_library(jackmeter_core
    src/Commands.cpp
    src/JackMeter.cpp
    src/CommandInvoker.cpp
    src/JackClient.cpp
    src/SimplePeakProcessor.cpp
    src/DiscontinuityDetector.cpp
)

# Link libraries to the core library
target_link_libraries(jackmeter_core
    ${wxWidgets_LIBRARIES}
    fmt::fmt
    ${JACK_LIBRARIES}
    ${SNDFILE_LIBRARIES}  # Link the SndFile library via pkg-config
    ${CURSES_LIBRARIES}    # Link ncurses library
    GSL::gsl 
    GSL::gslcblas
)

# Create the main executable and link it to the core library
add_executable(JackMeter main.cpp)
target_link_libraries(JackMeter jackmeter_core)

# -----------------------------------------------------
# Google Test and Google Mock integration
# -----------------------------------------------------
include(FetchContent)

# Download and configure GoogleTest and GoogleMock
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1  # Use the latest stable release
)

# Ensure the FetchContent module is made available
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add a new target for the test executable
add_executable(UnitTests
    tests/test_main.cpp
    tests/test_command.cpp
    tests/test_simple_peak_processor.cpp
    tests/test_jackclient.cpp
   # tests/test_integration.cpp
)

# Link GoogleTest, GoogleMock, and the core library to the test executable
target_link_libraries(UnitTests
    GTest::gtest_main
    GTest::gmock_main
    jackmeter_core
    fmt::fmt
    ${SNDFILE_LIBRARIES}  # Link SndFile library for tests
    ${CURSES_LIBRARIES}   # Link ncurses library for tests if needed
    GSL::gsl 
    GSL::gslcblas
)

# Register tests using GoogleTest
include(GoogleTest)
gtest_discover_tests(UnitTests)
