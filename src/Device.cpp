
#include "Device.hpp"

#include <iostream>

Device::Device(vk::raii::Instance* instance, Window* window): instance_(instance), window_(window) {

    std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Device constructor" << std::endl;
    createSurface();

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

bool Device::createLogicalDevice() {

    if(surface_ == nullptr) {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: cannot create logical device without a valid presentation surface." << std::endl;
        return false;
    }

    if(physicalDevice_ == nullptr) {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: cannot create logical device without a valid physical device." << std::endl;
        return false;
    }

    // specify queue family requirements
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice_.getQueueFamilyProperties();
    int32_t queueIndex = -1;
    for(uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++) {
        if((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDevice_.getSurfaceSupportKHR(qfpIndex, *surface_)) {
            queueIndex = static_cast<int32_t>(qfpIndex);
            break;
        }
    }
    if(queueIndex <= -1) {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: could not locate valid graphics queues." << std::endl;
        return false;
    }

    float queuePriority = 0.5f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo {
        .queueFamilyIndex = static_cast<uint32_t>(queueIndex),
        .queueCount = 1,
        .pQueuePriorities = &queuePriority
    };

    // specify device feature requirements
    vk::PhysicalDeviceVulkan13Features deviceFeatures = { .dynamicRendering = true };
    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT deviceStateFeatures = { . extendedDynamicState = true };
    vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> featureChain = {
        {}, // empty for now
        deviceFeatures,
        deviceStateFeatures
    };

    // create logical device
    vk::DeviceCreateInfo deviceCreateInfo {
        .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &deviceQueueCreateInfo,
        .enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensions_.size()),
        .ppEnabledExtensionNames = requiredDeviceExtensions_.data()
    };
    logicalDevice_ = vk::raii::Device(physicalDevice_, deviceCreateInfo);

    // initialize the graphics queue
    graphicsQueue_ = vk::raii::Queue(logicalDevice_, queueIndex, 0);

    if(logicalDevice_ != nullptr) {
        return true;
    } else {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: could not create a valid logical device." << std::endl;
        return false;
    }

}

void Device::createSurface() {

    (void)window_->createSurface(instance_, &surface_);

}
