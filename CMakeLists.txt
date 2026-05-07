
cmake_minimum_required(VERSION 3.10)

project(maiden)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

# add_subdirectory() to nest CMakeLists

add_executable(maiden
    src/main.cpp
    # include extra source files here
)

target_include_directories(maiden PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    # add additional include directories here
)

target_link_libraries(maiden PRIVATE
    # add libraries here
)
