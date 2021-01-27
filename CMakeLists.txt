cmake_minimum_required(VERSION 3.16)
project(mario_maker)

enable_testing()

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Setting Raylib

# Set this to the minimal version you want to support
find_package(raylib 3.5 QUIET) # Let CMake search for a raylib-config.cmake
find_package(TMXLITE REQUIRED)

# You could change the QUIET above to REQUIRED and remove this if() clause
# This part downloads raylib and builds it if it's not installed on your system
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)

    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/master.tar.gz
    )

    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)

        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

        # build raylib
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

    endif()

endif()

# Setting Google Test

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
        )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
        )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
        "${CMAKE_BINARY_DIR}/googletest-build"
        )

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
            "${gmock_SOURCE_DIR}/include"
            )
endif()

include_directories(cmake-build-debug/googletest-src/googlemock/include)
include_directories(cmake-build-debug/googletest-src/googletest/include)

file(GLOB src
        "src/*.cpp"
        "src/systems/*.cpp"
        "src/renderers/*.cpp")
add_executable(mario_maker ${src})
add_library(mario_maker_lib STATIC ${src})

include_directories(
        include
        include/systems
        include/renderers
        ${TMXLITE_INCLUDE_DIR})

target_link_libraries(mario_maker raylib ${TMXLITE_LIBRARIES} gtest gtest_main)

add_subdirectory(tests)
