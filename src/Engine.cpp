
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

    // get extensions that our windowing library requires
    uint32_t instanceExtensionsCount = 0;
    char const* const* instanceExtensions{ SDL_Vulkan_GetInstanceExtensions(&instanceExtensionsCount) };

    // get all available extensions
    auto extensionProperties = context_.enumerateInstanceExtensionProperties();

    // print if we feel like it
    std::cout << "Available Vulkan Extensions: " << std::endl;
    for(const auto& extensionProperty : extensionProperties) {
        std::cout << "\t" << extensionProperty.extensionName << std::endl;
    }

    // check that all required extensions are available
    for(uint32_t i = 0; i < instanceExtensionsCount; i++) { // for each extension that we require
        bool found = false;
        for(const auto& extensionProperty : extensionProperties) { // see if it matches any extensions that are provided
            if(strcmp(extensionProperty.extensionName, instanceExtensions[i]) == 0) {
                found = true;
                break;
            }
        }
	    if(!found) {
            std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Required SDL3 extension not supported: " << instanceExtensions[i] << std::endl;
            errorCount++;
        } else {
	        // in case you're curious
            //std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] SDL3 extension located: " << instanceExtensions[i] << std::endl;
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
    if(errorCount != 0) return false;

    vk::InstanceCreateInfo instanceCreateInfo {
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = instanceExtensionsCount,
        .ppEnabledExtensionNames = instanceExtensions,
    };

    instance_ = vk::raii::Instance(context_, instanceCreateInfo);
    return (instance_ != nullptr);
}

