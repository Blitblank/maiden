# Maiden

The maiden project is a GPU accelerated 3D rendering engine built with C++ based on the Vulkan API for an emphasis on optimization and performance. The engine is intended to:
- be usable cross platform
- use minimal external libraries
- be configurable
- be performance profilable

## Getting Started

### Clone Repository
```bash
$ git clone https://git.vxbard.net/homeburger/maiden.git

# If there's any necessary submodules then:
$ git clone --recurse-submodules https://git.vxbard.net/homeburger/maiden.git

# If you have already cloned the repository and you need its submodules:
$ git submodule update --init --recursive
```
### Build 
This app uses CMake and C++20, so a compatible compiler (gcc8, clang9, msvc16) are necessary.

Most library prerequisites are handled within CMake. Any others that need to be manually installed will be described below as the project grows.

To build the project (using default build directory "build"):
```bash
# configure
$ cmake -S . -B build # + any extra options

# build
$ cmake --build build -j

# execute
$ cd build
$ ./maiden
```

### Testing
This app uses GoogleTest for building unit tests. The below instructions use ctest and gcovr for test execution and code coverage:
```bash
# execute unit tests
$ ctest -T Test # + filter options

# analyze code coverage
$ cd build
$ gcovr -r .. --filter "../src"
```

### app troubleshooting here

## Development Roadmap
### lots of todo here
