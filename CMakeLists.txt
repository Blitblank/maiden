
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

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_CXX ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    libconfig
    GIT_REPOSITORY https://github.com/hyperrealm/libconfig.git
    GIT_TAG        v1.8.2
)
FetchContent_MakeAvailable(libconfig)

if(TARGET libconfig++)
    set(MAIDEN_LIBCONFIGXX_TARGET libconfig++)
elseif(TARGET config++)
    set(MAIDEN_LIBCONFIGXX_TARGET config++)
else()
    message(FATAL_ERROR "Could not find libconfig++ CMake target")
endif()

find_package(Vulkan REQUIRED)

# add_subdirectory() to nest CMakeLists

add_library(maiden_core STATIC
    src/App.cpp
    src/Window.cpp
    src/Engine.cpp
    src/Mesh.cpp
    src/Logger.cpp
    src/Device.cpp
    src/ConfigService.cpp
    src/Pipeline.cpp
    src/Swapchain.cpp
    # include extra source files here
)

add_executable(maiden
    src/main.cpp
)

add_executable(maiden_test
    test/TestApp.cpp
    test/TestConfigService.cpp
)

target_include_directories(maiden_core PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${SDL3_SOURCE_DIR}/include" # was having issues with cmake automatically adding these for a lib
    "$ENV{VULKAN_SDK}/include" # requires sourcing vulkan sdk setup-env.sh
    # add additional include directories here
)

target_compile_options(maiden_core PRIVATE --coverage)
target_link_options(maiden_core PRIVATE --coverage)
target_link_options(maiden PRIVATE --coverage)
target_link_options(maiden_test PRIVATE --coverage)

# vulkan necessity
target_compile_definitions(maiden_core PRIVATE VULKAN_HPP_NO_STRUCT_CONSTRUCTORS)

target_link_libraries(maiden PRIVATE
    maiden_core
    SDL3::SDL3
    Vulkan::Vulkan
)

target_link_libraries(maiden_core PUBLIC
    ${MAIDEN_LIBCONFIGXX_TARGET}
)

# test only stuff down here VVV
target_include_directories(maiden_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# TODO: add option for disabling tests (like if target == DEBUG then tests on, otherwise tests off)
target_link_libraries(maiden_test PRIVATE
    maiden_core
    GTest::gmock
    GTest::gmock_main
    SDL3::SDL3
    Vulkan::Vulkan
)

include(CTest)
enable_testing() # ctest is whatever

include(GoogleTest)
gtest_discover_tests(maiden_test)
