
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
    // check if these extensions are available
    auto extensionProperties = context_.enumerateInstanceExtensionProperties();
    uint32_t errorCount = 0;
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
            std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] SDL3 extension located: " << instanceExtensions[i] << std::endl;
        }
    } // if any weren't then we must exit
    if(errorCount != 0) return false;

    vk::InstanceCreateInfo instanceCreateInfo {
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = instanceExtensionsCount,
        .ppEnabledExtensionNames = instanceExtensions,
    };

    instance_ = vk::raii::Instance(context_, instanceCreateInfo);
    if(instance_ != nullptr) {
        return true;
    } else {
        return false;
    }
}

