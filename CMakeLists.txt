
cmake_minimum_required(VERSION 3.23)

project(maiden)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

# gtest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.x
)
FetchContent_MakeAvailable(googletest)

# sdl3
FetchContent_Declare(
    sdl3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.x
)
FetchContent_MakeAvailable(sdl3)

# add_subdirectory() to nest CMakeLists

add_executable(maiden
    src/App.cpp
    src/Window.cpp
    src/main.cpp
    # include extra source files here
)

add_executable(maiden_test
    src/App.cpp
    src/Window.cpp
    test/TestApp.cpp
)
# i think the strat is to build all of the core app components into a single library
# and then you link that to the two different executables (real main and test main)

target_include_directories(maiden PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    # add additional include directories here
)

target_link_libraries(maiden PRIVATE
    SDL3::SDL3
)

target_include_directories(maiden_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# test only stuff down here VVV
target_compile_options(maiden_test PRIVATE --coverage)
target_link_options(maiden_test PRIVATE --coverage)

# TODO: add option for disabling tests (like if target == DEBUG then tests on, otherwise tests off)
target_link_libraries(maiden_test PRIVATE
    GTest::gmock
    GTest::gmock_main
    SDL3::SDL3
)

include(CTest)
enable_testing() # ctest is whatever

include(GoogleTest)
gtest_discover_tests(maiden_test)
