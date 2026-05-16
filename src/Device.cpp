
#include "Device.hpp"

#include <iostream>

Device::Device(vk::raii::Instance* instance): instance_(instance) {

}

Device::~Device() {

}

bool Device::selectPhysicalDevice() {

    std::vector<vk::raii::PhysicalDevice> physicalDevices = instance_->enumeratePhysicalDevices();

    if(physicalDevices.empty()) {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: no physical devices with Vulkan support found." << std::endl;
        return false;
    }

    // validate found devices
    uint32_t maxScore = 0;
    for(vk::raii::PhysicalDevice& physicalDevice : physicalDevices) {
        uint32_t capabilityScore = evaluatePhysicalDevice(physicalDevice);
        if(capabilityScore > maxScore) {
            maxScore = capabilityScore;
            physicalDevice_ = physicalDevice;
        }
    }
    if(maxScore = 0) {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: physical devices found, but none capable for this engine." << std::endl;
        return false;
    } else {
        vk::PhysicalDeviceProperties deviceProperties = physicalDevice_.getProperties();
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Physical device selected: " << deviceProperties.deviceName << std::endl;
        return true;
    }

}

uint32_t Device::evaluatePhysicalDevice(vk::raii::PhysicalDevice& device) {

    vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
    vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();

    std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Physical device found: " << deviceProperties.deviceName << std::endl;

    if(deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
        return 2;
    } else {
        return 1;
    } // i.e. a descrete gpu is preferable to an integrated gpu

}
