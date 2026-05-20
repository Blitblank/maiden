
#pragma once

#include <vulkan/vulkan_raii.hpp>

class Device {

    public:

    Device(vk::raii::Instance* instance);
    ~Device();

    // assigns a capable gpu vkdevice to physicalDevice 
    bool selectPhysicalDevice();

    private:

    // gives a device a score to attempt to select the most capable device
    uint32_t evaluatePhysicalDevice(vk::raii::PhysicalDevice& device);

    vk::raii::Instance* instance_ = nullptr;
    vk::raii::PhysicalDevice physicalDevice_ = nullptr;

    // required extensions for the physical device
    std::vector<const char*> requiredDeviceExtensions_ = { vk::KHRSwapchainExtensionName };
    
};
