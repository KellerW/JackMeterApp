cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(JackMeter VERSION 1.0)

# Set common flags
set(COMMON_FLAGS
    -Wall
    -Wextra
    -Werror
)

set(COMMON_C_FLAGS
    ${COMMON_FLAGS}
    -std=c11
)

set(COMMON_CXX_FLAGS
    ${COMMON_FLAGS}
    -std=c++2a
)

# Set up C flags and C++ flags
string(REPLACE ";" " " CMAKE_COMMON_C_FLAGS "${COMMON_C_FLAGS}")
string(REPLACE ";" " " CMAKE_COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_COMMON_C_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${CMAKE_COMMON_CXX_FLAGS}" CACHE STRING "" FORCE)

# Allow reordering in C++ constructor (fix reorder warning)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=reorder")

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find wxWidgets (for GUI)
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Include directories for GUI and CLI
include_directories(${wxWidgets_INCLUDE_DIRS} include)

# Find FFTW3 using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTW3 REQUIRED fftw3)

if(FFTW3_FOUND)
    include_directories(${FFTW3_INCLUDE_DIRS})
    message(STATUS "FFTW3 includes: ${FFTW3_INCLUDE_DIRS}")
    message(STATUS "FFTW3 libraries: ${FFTW3_LIBRARIES}")
else()
    message(FATAL_ERROR "FFTW3 not found")
endif()

# Include GSL (GNU Scientific Library)
find_package(GSL REQUIRED)

# Find the fmt library
find_package(fmt REQUIRED)

# Find JACK and SndFile using pkg-config
pkg_check_modules(JACK REQUIRED jack)
pkg_check_modules(SNDFILE REQUIRED sndfile)

# Find ncurses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Include directories for JACK, SndFile, and WaveLib
include_directories(${JACK_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})
include_directories(/home/wagnerkeller/projects/wavelib/bin/include)

# Link directories for JACK, SndFile, and WaveLib
link_directories(${JACK_LIBRARY_DIRS} ${SNDFILE_LIBRARY_DIRS})
link_directories(/home/wagnerkeller/projects/wavelib/bin/lib)

# Add include directories for the project
include_directories(include)

# Create a library from core logic
add_library(jackmeter_core
    src/Commands.cpp
    src/CommandInvoker.cpp
    src/JackClient.cpp
    src/SimplePeakProcessor.cpp
    src/DiscontinuityDetector.cpp
    src/GnuplotHandler.cpp
    src/NcursesHandler.cpp
    src/FFTProcessor.cpp
    src/SignalHandler.cpp
    src/Utils.cpp
    src/AppContext.cpp
)

# Define header files (you can include more if necessary)
set(HEADER_FILES
    include/Commands.hpp
    include/CommandInvoker.hpp
    include/JackClient.hpp
    include/DiscontinuityDetector.hpp
)

# Link core library with required libraries
target_link_libraries(jackmeter_core
    ${wxWidgets_LIBRARIES}
    fmt::fmt
    ${JACK_LIBRARIES}
    ${SNDFILE_LIBRARIES}
    ${CURSES_LIBRARIES}
    GSL::gsl
    GSL::gslcblas
    fftw3
    ${FFTW3_LIBRARIES}
    wavelib
    wauxlib
)

# GUI source and header files
set(GUI_SRC_FILES
    src/MyApp.cpp
    src/MyFrame.cpp
)


# Create executables for CLI and GUI
add_executable(JackMeterCLI main.cpp)
add_executable(JackMeterGUI ${GUI_SRC_FILES}  ${SRC_FILES} ${HEADER_FILES})

# Link GUI executable with wxWidgets

target_link_libraries(JackMeterGUI ${wxWidgets_LIBRARIES} jackmeter_core)

# Link CLI executable with core logic
target_link_libraries(JackMeterCLI jackmeter_core)
#add_executable(JackMeter main.cpp)
#target_link_libraries(JackMeter jackmeter_core)
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

FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add unit test target
add_executable(UnitTests
    tests/test_main.cpp
    tests/test_command.cpp
    tests/test_simple_peak_processor.cpp
    tests/test_jackclient.cpp
    tests/test_FFTProcessor.cpp
    tests/test_discontinuity_processor.cpp
)

# Link unit tests with necessary libraries
target_link_libraries(UnitTests
    GTest::gtest_main
    GTest::gmock_main
    jackmeter_core
    fmt::fmt
    ${SNDFILE_LIBRARIES}
    ${CURSES_LIBRARIES}
    GSL::gsl
    GSL::gslcblas
    fftw3
    wavelib
    wauxlib
)

# Discover and register tests
include(GoogleTest)

gtest_discover_tests(UnitTests)
