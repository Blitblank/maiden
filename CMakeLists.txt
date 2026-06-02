
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

find_program(SLANGC_EXECUTABLE
    NAMES slangc
    HINTS "$ENV{VULKAN_SDK}/Bin" "$ENV{VULKAN_SDK}/bin"
    REQUIRED
)

find_package(Vulkan REQUIRED)

# add_subdirectory() to nest CMakeLists

set(MAIDEN_SHADER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/shaders/triangle.slang")
set(MAIDEN_SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
set(MAIDEN_SHADER_OUTPUT "${MAIDEN_SHADER_OUTPUT_DIR}/triangle.spv")

add_custom_command(
    OUTPUT "${MAIDEN_SHADER_OUTPUT}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${MAIDEN_SHADER_OUTPUT_DIR}"
    COMMAND "${SLANGC_EXECUTABLE}" "${MAIDEN_SHADER_SOURCE}"
            -target spirv
            -profile spirv_1_4
            -emit-spirv-directly
            -fvk-use-entrypoint-name
            -entry vertMain
            -entry fragMain
            -o "${MAIDEN_SHADER_OUTPUT}"
    DEPENDS "${MAIDEN_SHADER_SOURCE}"
    COMMENT "Compiling Slang shaders"
    VERBATIM
)

add_custom_target(maiden_shaders DEPENDS "${MAIDEN_SHADER_OUTPUT}")

add_library(maiden_core STATIC
    src/App.cpp
    src/Window.cpp
    src/Engine.cpp
    src/Mesh.cpp
    src/Logger.cpp
    src/PPS.cpp
    src/Device.cpp
    src/ConfigService.cpp
    src/Pipeline.cpp
    src/ShaderModule.cpp
    src/Swapchain.cpp
    # include extra source files here
)

add_executable(maiden
    src/main.cpp
)

add_executable(maiden_test
    test/TestApp.cpp
    test/TestConfigService.cpp
    test/TestShaderModule.cpp
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
target_compile_definitions(maiden_core PRIVATE
    VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
    BINARY_DIR="${CMAKE_BINARY_DIR}"    
)
target_compile_definitions(maiden_core PUBLIC MAIDEN_SHADER_DIR="${MAIDEN_SHADER_OUTPUT_DIR}")

target_link_libraries(maiden PRIVATE
    maiden_core
    SDL3::SDL3
    Vulkan::Vulkan
)

target_link_libraries(maiden_core PUBLIC
    ${MAIDEN_LIBCONFIGXX_TARGET}
)
add_dependencies(maiden maiden_shaders)

# test only stuff down here VVV
target_include_directories(maiden_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_definitions(maiden_test PRIVATE MAIDEN_TEST_SHADER_PATH="${MAIDEN_SHADER_OUTPUT}")

# TODO: add option for disabling tests (like if target == DEBUG then tests on, otherwise tests off)
target_link_libraries(maiden_test PRIVATE
    maiden_core
    GTest::gmock
    GTest::gmock_main
    SDL3::SDL3
    Vulkan::Vulkan
)

add_dependencies(maiden_test maiden_shaders)

include(CTest)
enable_testing() # ctest is whatever

include(GoogleTest)
gtest_discover_tests(maiden_test)
