
#pragma once

// resource allocation is initializion my beloved
#include <vulkan/vulkan_raii.hpp>

#include "Window.hpp"
#include "Logger.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"
#include "Pipeline.hpp"

class Engine {

public:

    Engine(Window* window, Logger* logger);
    ~Engine() = default;

    // initializes and sets up the vulkan instance. outside of constructor to allow control of order the order of initialization
    void init();
    
    // draw is called every render iteration in that while loop, handles syncronization
    bool drawFrame();

private:

    // initiates and submits a draw call
    bool render(uint32_t imageIndex);

    // returns a list of the required extensions needed by our engine
    std::vector<const char*> getRequiredInstanceExtensions();

    // helper for organizing the creation of the vulkan instance. returns success or failure
    bool createInstance();

    // helper for attaching callbacks to the instance
    bool initDebugMessenger();

    // write graphics commands
    bool createCommandBuffer();
    bool recordCommandBuffer(uint32_t imageIndex);
    vk::raii::CommandBuffer commandBuffer_ = nullptr;

    // helper for syncronization instantiation
    void createSyncObjects();

    // helper for swapping images
    void transitionImageLayout(uint32_t imageIndex, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::AccessFlags2 srcAccessMask,
        vk::AccessFlags2 destAccessMask, vk::PipelineStageFlags2 srcStageMask, vk::PipelineStageFlags2 destStageMask);

    // callback function for the vulkan debug extension: routes debug messages from validation layers to our app
    static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                          vk::DebugUtilsMessageTypeFlagsEXT type,
                                                          const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                          void* pUserData);

    Logger* logger_ = nullptr;
    Window* window_ = nullptr;
    Device* device_ = nullptr;
    Swapchain* swapchain_ = nullptr;
    Pipeline* pipeline_ = nullptr;

    // Vulkan specific instance members
    vk::raii::Context context_;
    vk::raii::Instance instance_ = nullptr;
    vk::raii::DebugUtilsMessengerEXT debugMessenger_ = nullptr;

    vk::raii::Semaphore presentCompleteSemaphore_ = nullptr;
    vk::raii::Semaphore renderCompleteSemaphore_ = nullptr;
    vk::raii::Fence drawFence_ = nullptr;

    // members for control over validation layers
    static constexpr bool enableValidationLayers = true; // TODO: only true in debug mode
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

};
