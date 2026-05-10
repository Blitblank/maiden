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

### Installation Prequisites
LunarG's Vulkan SDK is used for almost every Vulkan project, including this one.
Installation instructions here are for Ubuntu 24, further instructions can be found here: https://vulkan.lunarg.com/doc/sdk/1.4.341.1/linux/getting_started.html
```bash
$ cd ~/Downloads
$ wget "https://sdk.lunarg.com/sdk/download/1.4.341.1/linux/vulkansdk-linux-x86_64-1.4.341.1.tar.xz" # or the latest version on https://vulkan.lunarg.com/sdk/home
$ mkdir /opt/vulkan-sdk
$ cd /opt/vulkan-sdk
$ tar -xvf ~/Downloads/vulkansdk-linux-x86_64-1.4.341.1.tar.xz
$ sudo apt install libxcb-xinput0 libxcb-xinerama0 libxcb-cursor-dev # vulkan prerequeisites
```
Then to setup your current session:
```bash
$ source /opt/vulkan-sdk/1.4.341.1/setup-env.sh # or whichever version you have
```
Note: sourcing only affects your current shell session. Add to your .bashrc script if you wish for it to persist.

Verify your installation by using the the command:
```bash
$ vulkaninfo
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
