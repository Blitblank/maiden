
#include "Engine.hpp"

#include <iostream>

Engine::Engine(Window* window): window_(window) {

    // cleans up this constructor 
    init();

}

void Engine::init() {

    if(createInstance()) {
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
    std::cout << "Available Vulkan Extensions: " << std::endl;
    for(const auto& extensionProperty : extensionProperties) {
        std::cout << "\t" << extensionProperty.extensionName << std::endl;
    } // this would be a logger.debug(...)

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
            std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Required SDL3 extension not supported: " << requiredInstanceExtensions[i] << std::endl;
            errorCount++;
        } else {
	        // in case you're curious
            //std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] SDL3 extension located: " << requiredInstanceExtensions[i] << std::endl;
        }
    }

    // get required validation layers as specified by our app
    std::vector<const char*> requiredValidationLayers;
    if(enableValidationLayers) requiredValidationLayers.assign(validationLayers.begin(), validationLayers.end());

    // get available validation layers
    auto validationLayerProperties = context_.enumerateInstanceLayerProperties();

    // again print if we feel like it
    std::cout << "Available Vulkan Validation Layers: " << std::endl;
    for(const auto& validationLayer : validationLayerProperties) {
        std::cout << "\t" << validationLayer.layerName << std::endl;
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
            std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Required validation layer not supported: " << requiredValidationLayers[i] << std::endl;
        } else { // in case you're curious
            //std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] VkValidation layer located: " << requiredValidationLayers[i] << std::endl;
        }
    }
    
    // if any we had errors then we must exit
    if(errorCount != 0) {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Unable to create Vulkan instance. Error count: " << errorCount << std::endl;
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
    const char* const* sdlExtensions{ SDL_Vulkan_GetInstanceExtensions(&sdlExtensionsCount) };
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

    // this will eventually go through our logger
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

