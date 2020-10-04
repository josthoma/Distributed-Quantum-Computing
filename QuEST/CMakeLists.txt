# author: Ania Brown 
# author: Jacob Wilkins
# author: Balint Koczor (Windows compatibility)
# author: Tyson Jones (testing)

# CMake initialisation.
cmake_minimum_required(VERSION 3.1)

message(STATUS "CMake version: ${CMAKE_VERSION}")
message(STATUS "CMake build type: ${CMAKE_BUILD_TYPE}")

# Project name
project(QuEST_Project)


# GDB debug
# SET(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_BUILD_TYPE Debug)


# -----------------------------------------------------------------------------
# ----- USER OPTIONS ----------------------------------------------------------
# -----------------------------------------------------------------------------

# set(USER_SOURCE  "examples/tutorial_example.c"  CACHE STRING "Source to build with QuEST library")
# set(OUTPUT_EXE   "demo"  CACHE STRING "Executable to compile to")


set(USER_SOURCE  testCode/eceBenchmark.c testCode/newRandomCircuits.c testCode/helper.c CTools/memorymeasure.c CACHE STRING "Source 1 to build with QuEST library")
set(OUTPUT_EXE   "eceBenchmark"  CACHE STRING "Executable to compile to")

option(TESTING "Enable unit testing. This disables compiling USER_SOURCE" OFF)

message(STATUS "============== Check 1 ============")
message(STATUS "============== ${USER_SOURCE}")
message(STATUS "============== ${OUTPUT_EXE}")

# -----------------------------------------------------------------------------
# ----- QuEST LIBRARY ---------------------------------------------------------
# ----------------------------------------------------------------------------- 

# Build the QuEST library if the path to libQuEST.so is not specified
if (NOT DEFINED ${QuEST_LIB_PATH})
	message(STATUS "============== Here 1 ============")
    # Build libQuEST.so
    set(QuEST_DIR "QuEST" CACHE STRING 
        "Name of the directory containing the QuEST library sources. It must be located in the same directory as the root CMakeLists.txt")
    add_subdirectory(${QuEST_DIR})
    set(QuEST_LIB_PATH "${CMAKE_CURRENT_BINARY_DIR}/${QuEST_DIR}")
    set(QuEST_LIB_EXACT "${QuEST_LIB_PATH}/libQuEST.so")

	message(STATUS "============== ${QuEST_DIR}")
	message(STATUS "============== ${QuEST_LIB_PATH}")
	message(STATUS "============== ${QuEST_LIB_EXACT}")
endif()

# -----------------------------------------------------------------------------
# ----- USER EXECUTABLE -------------------------------------------------------
# -----------------------------------------------------------------------------

if (NOT TESTING)
    message(STATUS "Compiling ${USER_SOURCE} to executable ${OUTPUT_EXE}")

    # Create user executable
    # add_executable(${OUTPUT_EXE} ${USER_SOURCE_1} ${USER_SOURCE_2})
    add_executable(${OUTPUT_EXE} ${USER_SOURCE})

    # Link libraries to user executable, including QuEST library
    if (WIN32)
        target_link_libraries(${OUTPUT_EXE} QuEST)
    else ()
		message(STATUS "============== Here ============")
        target_link_libraries(${OUTPUT_EXE} QuEST m)
    endif()
endif()

# -----------------------------------------------------------------------------
# ----- TESTS -----------------------------------------------------------------
# -----------------------------------------------------------------------------

if (TESTING)
    message(STATUS "Compiling unit tests")

    enable_testing()
    add_subdirectory(tests)
endif()

message(STATUS "===== what is QuEST_SRC ${QuEST_SRC}")
