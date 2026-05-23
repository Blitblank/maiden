
#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "Window.hpp"

class Device { 

    public:

    Device(vk::raii::Instance* instance, Window* window);
    ~Device();

    // assigns a capable gpu vkdevice to physicalDevice 
    bool selectPhysicalDevice();

    // initializes the logical device
    bool createLogicalDevice();

    private:

    // gives a device a score to attempt to select the most capable device
    uint32_t evaluatePhysicalDevice(vk::raii::PhysicalDevice& device);

    // internal create surface
    void createSurface();

    // vulkan objects
    vk::raii::Instance* instance_ = nullptr;
    vk::raii::PhysicalDevice physicalDevice_ = nullptr;
    vk::raii::Device logicalDevice_ = nullptr;
    vk::raii::Queue graphicsQueue_ = nullptr;
    vk::raii::SurfaceKHR surface_ = nullptr;

    // ptrs to other engine objects
    Window* window_ = nullptr;

    // required extensions for the physical device
    std::vector<const char*> requiredDeviceExtensions_ = { vk::KHRSwapchainExtensionName };
    
};
