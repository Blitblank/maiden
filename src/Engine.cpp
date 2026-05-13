
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

    // get necessary extensions that our windowing library requires
    uint32_t instanceExtensionsCount = 0;
    char const* const* instanceExtensions{ SDL_Vulkan_GetInstanceExtensions(&instanceExtensionsCount) };

    // get required validation layers as specified by our app
    std::vector<const char*> requiredValidationLayers;
    if(enableValidationLayers) requiredValidationLayers.assign(validationLayers.begin(), validationLayers.end());

    auto validationLayerProperties = context_.enumerateInstanceLayerProperties();
    auto unsupportedLayer = std::ranges::find_if(requiredValidationLayers, [&validationLayerProperties](const auto& requiredLayer) {
        return std::ranges::none_of(validationLayerProperties, [requiredLayer](const auto& layerProperty) {
            return strcmp(layerProperty.layerName, requiredLayer) == 0;
        });
    }); // TODO: what black magic even is this
    if(unsupportedLayer != requiredValidationLayers.end()) {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Required validation layer not supported: " << *unsupportedLayer << std::endl;
        errorCount++;
    }

    // get all available extensions
    auto extensionProperties = context_.enumerateInstanceExtensionProperties();

    // print if we feel like it
    std::cout << "Available Vulkan Extensions: " << std::endl;
    for(const auto& extensionProperty : extensionProperties) {
        std::cout << "\t" << extensionProperty.extensionName << std::endl;
    }

    for(uint32_t i = 0; i < instanceExtensionsCount; i++) { // for each extension that we require
        bool found = false;
        for(const auto& extensionProperty : extensionProperties) { // see if it matches any extensions that are provided
            if(strcmp(extensionProperty.extensionName, instanceExtensions[i]) == 0) {
                std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Required SDL3 extension not supported: " << instanceExtensions[i] << std::endl;
            } else {
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
    } // if any weren't then we must exit
    if(errorCount != 0) return false;

    vk::InstanceCreateInfo instanceCreateInfo {
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = instanceExtensionsCount,
        .ppEnabledExtensionNames = instanceExtensions,
    };

    instance_ = vk::raii::Instance(context_, instanceCreateInfo);
    return (instance_ != nullptr);
}

