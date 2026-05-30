
#pragma once

// resource allocation is initializion my beloved
#include <vulkan/vulkan_raii.hpp>

#include "Window.hpp"

class Engine {

public:

    Engine(Window* window, Logger* logger);
    ~Engine() = default;

    // initializes and sets up the vulkan instance. outside of constructor to allow control of order the order of initialization
    void init();
    
    // draw is called every render iteration in that while loop
    void draw();

private:

    // returns a list of the required extensions needed by our engine
    std::vector<const char*> getRequiredInstanceExtensions();

    // helper for organizing the creation of the vulkan instance. returns success or failure
    bool createInstance();

    // helper for attaching callbacks to the instance
    bool initDebugMessenger();

    // callback function for the vulkan debug extension: routes debug messages from validation layers to our app
    static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                          vk::DebugUtilsMessageTypeFlagsEXT type,
                                                          const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                          void* pUserData);

    Logger* logger_;
    Window* window_;

    // Vulkan specific instance members
    vk::raii::Context context_;
    vk::raii::Instance instance_ = nullptr;
    vk::raii::DebugUtilsMessengerEXT debugMessenger_ = nullptr;

    // members for control over validation layers
    static constexpr bool enableValidationLayers = true; // TODO: only true in debug mode
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

};
