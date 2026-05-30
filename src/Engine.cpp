
#include "Engine.hpp"

#include <iostream>

#include "Device.hpp"
#include "Swapchain.hpp"

Engine::Engine(Window* window, Logger* logger): window_(window), logger_(logger) {

    // cleans up this constructor 
    init();

}

void Engine::init() {

    if(createInstance()) {
        logger_->log("Engine", LogFlag::Info, "Vulkan instance successfully created.");
    } else {
        logger_->log("Engine", LogFlag::Error, "Unable to create Vulkan instance.");
    }

    // device selection and setup
    Device device(&instance_, window_, logger_);

    // render pipeline
    Swapchain swapchain(&device);
    // Pipeline pipeline(&device, &swapchain);

}

void Engine::draw() {

}

bool Engine::createInstance() {

    uint32_t errorCount = 0;

    // create the appInfo filled with information about our app
    constexpr vk::ApplicationInfo appInfo { // using the c++ api instead of the c api
        .pApplicationName = "maiden",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "null",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_4 // this one is most important
    };

    std::vector<const char*> requiredInstanceExtensions = getRequiredInstanceExtensions();

    // get all available extensions
    auto extensionProperties = context_.enumerateInstanceExtensionProperties();

    // print if we feel like it
    logger_->log("Engine", LogFlag::Debug, "Available Vulkan Extensions:");
    for(const auto& extensionProperty : extensionProperties) {
        // TODO: log function probably could do with variable arguments and string formatting because this is slightly clunky
        std::string msg = "\t" + std::string(extensionProperty.extensionName);
        logger_->log("Engine", LogFlag::Debug, msg);
    }

    // check that all required extensions are available
    for(uint32_t i = 0; i < requiredInstanceExtensions.size(); i++) { // for each extension that we require
        bool found = false;
        for(const auto& extensionProperty : extensionProperties) { // see if it matches any extensions that are provided
            if(strcmp(extensionProperty.extensionName, requiredInstanceExtensions[i]) == 0) {
                found = true;
                break;
            }
        }
	    if(!found) {
            std::string msg = "Required SDL3 extension not supported: " + std::string(requiredInstanceExtensions[i]);
            logger_->log("Engine", LogFlag::Warning, msg);
            errorCount++;
        } else {
	        // in case you're curious
            std::string msg = "SDL3 extension located: " + std::string(requiredInstanceExtensions[i]);
            logger_->log("Engine", LogFlag::Debug, msg);
        }
    }

    // get required validation layers as specified by our app
    std::vector<const char*> requiredValidationLayers;
    if(enableValidationLayers) requiredValidationLayers.assign(validationLayers.begin(), validationLayers.end());

    // get available validation layers
    auto validationLayerProperties = context_.enumerateInstanceLayerProperties();

    // again print if we feel like it
    logger_->log("Engine", LogFlag::Debug, "Available Vulkan Validation Layers:");
    for(const auto& validationLayer : validationLayerProperties) {
        std::string msg = "\t" + std::string(validationLayer.layerName);
        logger_->log("Engine", LogFlag::Debug, msg);
    }

    // check that all required validation layers are avilable
    for(int i = 0; i < requiredValidationLayers.size(); i++) {
        bool found = false;
        for(const auto& validationLayer : validationLayerProperties) {
            if(strcmp(requiredValidationLayers[i], validationLayer.layerName) == 0) {
                found = true;
                break;
            }
        }
        if(!found) {
            errorCount++;
            std::string msg = "Required validation layer not supported: " + std::string(requiredValidationLayers[i]);
            logger_->log("Engine", LogFlag::Warning, msg);
        } else { // in case you're curious
            std::string msg = "VkValidation layer located: " + std::string(requiredValidationLayers[i]);
            logger_->log("Engine", LogFlag::Debug, msg);
        }
    }
    
    // if any we had errors then we must exit
    if(errorCount != 0) {
        std::string msg = "Unable to create Vulkan instance. Error count: " + std::to_string(errorCount);
        logger_->log("Engine", LogFlag::Debug, msg);
        return false;
    }

    vk::InstanceCreateInfo instanceCreateInfo {
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = static_cast<uint32_t>(requiredValidationLayers.size()),
        .ppEnabledLayerNames = requiredValidationLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(requiredInstanceExtensions.size()),
        .ppEnabledExtensionNames = requiredInstanceExtensions.data()
    };

    instance_ = vk::raii::Instance(context_, instanceCreateInfo);
    return (instance_ != nullptr);
}

std::vector<const char*> Engine::getRequiredInstanceExtensions() {

    // get extensions that our windowing library requires
    uint32_t sdlExtensionsCount = 0;
    const char* const* sdlExtensions{ SDL_Vulkan_GetInstanceExtensions(&sdlExtensionsCount) }; // TODO: get this from window so all sdl3 is encapsulated there
    // what in the world is this kind of pointer btw

    std::vector<const char*> requiredExtensions(sdlExtensions, sdlExtensions + static_cast<size_t>(sdlExtensionsCount));

    // manually add an extension for handling validation layers
    if(enableValidationLayers) {
        requiredExtensions.push_back(vk::EXTDebugUtilsExtensionName);
    }

    return requiredExtensions;
}

bool Engine::initDebugMessenger() {

    if(!enableValidationLayers) return false;

    // masks for which debug messages we want to see
    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
    vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{.messageSeverity = severityFlags,
                                                                          .messageType     = messageTypeFlags,
                                                                          .pfnUserCallback = &debugCallback};
    debugMessenger_ = instance_.createDebugUtilsMessengerEXT( debugUtilsMessengerCreateInfoEXT );
    // we could get rid of this and just pass all the control to the logger
    // like: "treat all messages of severity vk::eVerbose as our own DebugVerbosity::Info"

    return (debugMessenger_ != nullptr);
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL Engine::debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                              vk::DebugUtilsMessageTypeFlagsEXT type,
                                                              const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                              void* pUserData) {

    // validation layers are for debugging only (and also its a pain hooking a nonstatic object to a static function)
    std::cout << "[ Validation Layer ] [Type: " << to_string(type) << "] " <<  pCallbackData->pMessage << std::endl;
    /*
    vk severity types:
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eError

    vk message types:
    vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
    vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
    vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
    */

    // returns whether or not we should abort, we'll always say no
    return vk::False;
}

