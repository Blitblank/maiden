
#include "Engine.hpp"

#include "vulkan/vulkan.h"
#include <iostream>

Engine::Engine(Window* window): window_(window) {

    // cleans up this constructor 
    init();

}

void Engine::init() {

    VkApplicationInfo appInfo {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "maiden",
        .apiVersion = VK_API_VERSION_1_4
    };

    uint32_t instanceExtensionsCount = 0;
    char const* const* instanceExtensions{ SDL_Vulkan_GetInstanceExtensions(&instanceExtensionsCount) };

    VkInstanceCreateInfo instanceCI {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = instanceExtensionsCount,
        .ppEnabledExtensionNames = instanceExtensions,
    };

    VkInstance instance;
    if(vkCreateInstance(&instanceCI, nullptr, &instance) == VK_SUCCESS) {
        // TODO: need some kind of logger service
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Vulkan instance successfully created." << std::endl;
    } else {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error creating Vulkan instance." << std::endl;
    }

    // next steps:
    // device selection and setup
    // queue creation
    // vulkan memory allocator
    // create vulkan surface
    // attach surface to window

}

void Engine::draw() {

}
