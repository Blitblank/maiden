
#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "Window.hpp"
<<<<<<< HEAD
=======
#include "Logger.hpp"
>>>>>>> 01992b54c3fd6f2627cdee9be7f7b2703cd7957d

class Device { 

    public:

<<<<<<< HEAD
    Device(vk::raii::Instance* instance, Window* window);
=======
    Device(vk::raii::Instance* instance, Window* window, Logger* logger);
>>>>>>> 01992b54c3fd6f2627cdee9be7f7b2703cd7957d
    ~Device();

    // helper to get the surface extent from the window
    bool getExtent(int32_t* width, int32_t* height);
    vk::raii::SurfaceKHR* getSurface() { return &surface_; }

    vk::raii::PhysicalDevice physicalDevice() { return physicalDevice_; }
    vk::raii::Device* logicalDevice() { return &logicalDevice_; }

    private:

    // gives a device a score to attempt to select the most capable device
    uint32_t evaluatePhysicalDevice(vk::raii::PhysicalDevice& device);

    // creates the surface
    void createSurface();

    // assigns a capable gpu vkdevice to physicalDevice 
    bool selectPhysicalDevice();

    // initializes the logical device
    bool createLogicalDevice();

    // vulkan objects
    vk::raii::Instance* instance_ = nullptr;
    vk::raii::PhysicalDevice physicalDevice_ = nullptr;
    vk::raii::Device logicalDevice_ = nullptr;
    vk::raii::Queue graphicsQueue_ = nullptr;
    vk::raii::SurfaceKHR surface_ = nullptr;

    // ptrs to other engine objects
    Window* window_ = nullptr;
<<<<<<< HEAD
=======
    Logger* logger_ = nullptr;
>>>>>>> 01992b54c3fd6f2627cdee9be7f7b2703cd7957d

    // required extensions for the physical device
    std::vector<const char*> requiredDeviceExtensions_ = { vk::KHRSwapchainExtensionName };
    
};
