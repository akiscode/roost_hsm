cmake_minimum_required(VERSION 3.5)

project(RoostHsm LANGUAGES CXX)

# Options

option(BUILD_TESTS "Build Tests" ON)
option(BUILD_BENCH "Build Benchmark Suite" ON)
option(BUILD_EXAMPLES "Build the examples folder" ON)

option(DEBUG_BUILD "Build in Debug" OFF)

# This setting is to create the production build to be pulled in by other projects
# A production build:
# - Turns off debug builds
# - Doesn't build benchmarks or examples
# - Builds tests, but does not package them
option(PRODUCTION_BUILD "Build with production settings" OFF)

if(PRODUCTION_BUILD)

    set(BUILD_BENCH OFF)
    set(BUILD_EXAMPLES OFF)

    set(DEBUG_BUILD OFF)

endif()

MESSAGE("PRODUCTION BUILD? " ${PRODUCTION_BUILD})
MESSAGE("BUILD TESTS? " ${BUILD_TESTS})
MESSAGE("BUILD BENCH? " ${BUILD_BENCH})
MESSAGE("BUILD EXAMPLES? " ${BUILD_EXAMPLES})
MESSAGE("DEBUG BUILD? " ${DEBUG_BUILD})

# Project Info 

set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# C++ Version
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------

if (DEBUG_BUILD)
    set(CMAKE_BUILD_TYPE Debug)
    set(PACKAGE_TYPE_STR debug)

elseif(PRODUCTION_BUILD)
    set(CMAKE_BUILD_TYPE Release)
    set(PACKAGE_TYPE_STR prod)

else()
    set(CMAKE_BUILD_TYPE Release)
    set(PACKAGE_TYPE_STR dev)
endif()

include(GNUInstallDirs)
include(ExternalProject)
include(CTest)

if (BUILD_TESTS)
    enable_testing()
    include(cmake/googletest.cmake)
endif()

if (BUILD_BENCH)
    include(cmake/hayai.cmake) 
endif()

# Setup Apps
add_subdirectory(lib)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


# Package Info
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})

set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${PACKAGE_TYPE_STR}")

# Include Cpack after setting variables for them to take effect
include(CPack)
