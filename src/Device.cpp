
#include "Device.hpp"

Device::Device(vk::raii::Instance* instance, Window* window, Logger* logger): instance_(instance), window_(window), logger_(logger) {

    selectPhysicalDevice();
    createSurface();
    createLogicalDevice();
    createCommandPool();

}

Device::~Device() {

}

bool Device::selectPhysicalDevice() {

    std::vector<vk::raii::PhysicalDevice> physicalDevices = instance_->enumeratePhysicalDevices();

    if(physicalDevices.empty()) {
        logger_->log("Device", LogFlag::Error, "No physical devices with Vulkan support found.");
        return false;
    }

    // validate found devices
    int32_t maxScore = 0;
    for(vk::raii::PhysicalDevice& physicalDevice : physicalDevices) {
        int32_t capabilityScore = evaluatePhysicalDevice(physicalDevice);
        if(capabilityScore > maxScore) {
            maxScore = capabilityScore;
            physicalDevice_ = physicalDevice;
        }
    }
    if(maxScore = 0) {
        logger_->log("Device", LogFlag::Error, "Physical devices found, but none capable for this engine.");
        return false;
    } else {
        vk::PhysicalDeviceProperties deviceProperties = physicalDevice_.getProperties();
        std::string deviceName = (deviceProperties.deviceName).data();
        std::string msg = "Physical device selected: " + deviceName;
        logger_->log("Device", LogFlag::Info, msg);
        return true;
    }

}

int32_t Device::evaluatePhysicalDevice(vk::raii::PhysicalDevice& device) {

    vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
    vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();

    std::string deviceName = (deviceProperties.deviceName).data();
    std::string msg = "Physical device found: " + deviceName;
    logger_->log("Device", LogFlag::Debug, msg);

    int32_t score = 0;

    // TODO: this is very basic and can be improved

    // prefer discrete graphics to integrated graphics
    if(deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
        score += 2;
    } else {
        std::string msg = "Warning: physical device " + deviceName + " is not a discrete device!";
        logger_->log("Device", LogFlag::Debug, msg);
    }

    // prefer devices that support vulkan 1.3
    if(deviceProperties.apiVersion >= vk::ApiVersion13) {
        score++;
    } else {
        std::string msg = "Warning: physical device " + deviceName + " does not support >= Vulkan 1.3!";
        logger_->log("Device", LogFlag::Debug, msg);
    }

    // prefer devices that support graphics queues
    auto queueFamilies = device.getQueueFamilyProperties();
    if(std::ranges::any_of( queueFamilies, []( auto const & qfp ) { return !!( qfp.queueFlags & vk::QueueFlagBits::eGraphics ); } )) {
        score++;
    } else {
        std::string msg = "Warning: physical device " + deviceName + " does not support graphics queue families!";
        logger_->log("Device", LogFlag::Debug, msg);
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
        std::string msg = "Warning: physical device " + deviceName + " is missing extensions!";
        logger_->log("Device", LogFlag::Debug, msg);
    }

    // prefer devices that support all required features
    auto features = device.template getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
    if(features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering
        && features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState
        && features.template get<vk::PhysicalDeviceVulkan13Features>().synchronization2 ) {
        score += 3; // this is most important
    } else {
        std::string msg = "Warning: physical device " + deviceName + " is missing features!";
        logger_->log("Device", LogFlag::Debug, msg);
    }

    return score;
}

bool Device::createLogicalDevice() {

    if(surface_ == nullptr) {
        logger_->log("Device", LogFlag::Error, "Cannot create logical device without a valid presentation surface.");
        return false;
    }

    if(physicalDevice_ == nullptr) {
        logger_->log("Device", LogFlag::Error, "Cannot create logical device without a valid physical device.");
        return false;
    }

    // specify queue family requirements
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice_.getQueueFamilyProperties();
    for(uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++) {
        if((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDevice_.getSurfaceSupportKHR(qfpIndex, *surface_)) {
            queueIndex_ = static_cast<int32_t>(qfpIndex);
            break;
        }
    }
    if(queueIndex_ <= -1) {
        logger_->log("Device", LogFlag::Error, "Could not locate valid graphics queues.");
        return false;
    }

    float queuePriority = 0.5f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo {
        .queueFamilyIndex = static_cast<uint32_t>(queueIndex_),
        .queueCount = 1,
        .pQueuePriorities = &queuePriority
    };

    // specify device feature requirements
    vk::PhysicalDeviceVulkan11Features vulkan11Features = { .shaderDrawParameters = true };
    vk::PhysicalDeviceVulkan13Features vulkan13Features = { .synchronization2 = true, .dynamicRendering = true };
    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT deviceStateFeatures = { . extendedDynamicState = true };
    vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> featureChain = {
        {}, // empty for now
        vulkan11Features,
        vulkan13Features,
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
    graphicsQueue_ = vk::raii::Queue(logicalDevice_, queueIndex_, 0);

    if(logicalDevice_ != nullptr) {
        logger_->log("Device", LogFlag::Info, "Created logcal device");
        return true;
    } else {
        logger_->log("Device", LogFlag::Error, "Could not create a valid logical device.");
        return false;
    }

}

void Device::createSurface() {

    (void)window_->createSurface(instance_, &surface_);

    if(surface_ == nullptr) {
        logger_->log("Device", LogFlag::Error, "Unable to create surface!");
        return;
    }

    if(physicalDevice_ == nullptr) {
        logger_->log("Device", LogFlag::Error, "Cannot attach surface without a physical device!");
        return;
    }

    auto surfaceCapabilities = physicalDevice_.getSurfaceCapabilitiesKHR(*surface_);
    std::vector<vk::SurfaceFormatKHR> availableFormats = physicalDevice_.getSurfaceFormatsKHR(*surface_);
    std::vector<vk::PresentModeKHR> availablePresentModes = physicalDevice_.getSurfacePresentModesKHR(*surface_);

}

bool Device::getExtent(int32_t* width, int32_t* height) {
    return window_->getExtent(width, height);
}

bool Device::createCommandPool() {

    if(queueIndex_ < 0) {
        logger_->log("Device", LogFlag::Error, "Cannot create command pool with invalid queueIndex.");
        return false;
    }

    vk::CommandPoolCreateInfo poolInfo {
        .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        .queueFamilyIndex = static_cast<uint32_t>(queueIndex_)
    };

    commandPool_ = vk::raii::CommandPool(logicalDevice_, poolInfo);

    if(commandPool_ == nullptr) {
        logger_->log("Device", LogFlag::Error, "Could not create command pool.");
        return false;
    } else {
        return true;
    }
}
