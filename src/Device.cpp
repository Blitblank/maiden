
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

    uint32_t score = 0;

    // TODO: this is very basic and can be improved

    // prefer discrete graphics to integrated graphics
    if(deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
        score += 2;
    } else {
         std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Warning: physical device " << deviceProperties.deviceName << " is not a discrete device!" << std::endl;
    }

    // prefer devices that support vulkan 1.3
    if(deviceProperties.apiVersion >= vk::ApiVersion14) {
        score++;
    } else {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Warning: physical device " << deviceProperties.deviceName << " does not support Vulkan 1.3! (" << std::endl;
    }

    // prefer devices that support graphics queues
    auto queueFamilies = device.getQueueFamilyProperties();
    if(std::ranges::any_of( queueFamilies, []( auto const & qfp ) { return !!( qfp.queueFlags & vk::QueueFlagBits::eGraphics ); } )) {
        score++;
    } else {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Warning: physical device " << deviceProperties.deviceName << " does not support graphics queue families!" << std::endl;
    }

    // prefer devices that support all required extensions
    auto availableDeviceExtensions = device.enumerateDeviceExtensionProperties();
    bool found = false;
    uint32_t missingExtensions = 0;
    for(auto& requiredExtension : requiredDeviceExtensions_) {
        for(auto& availableExtension: availableDeviceExtensions) {
            if(strcmp(availableExtension.extensionName, requiredExtension) == 0) {
                found = true;
                continue;
            }
        }
        if(found == false) {
            missingExtensions++;
        }
    }
    if(missingExtensions == 0) {
        score++;
    } else {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Warning: physical device " << deviceProperties.deviceName << " is missing extensions!" << std::endl;
    }

    // prefer devices that support all required features
    auto features = device.template getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
    if(features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering && features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState) {
        score++;
    } else {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Warning: physical device " << deviceProperties.deviceName << " is missing features!" << std::endl;
    }

    return score;
}
