
#pragma once

#include "Device.hpp"
#include "Swapchain.hpp"

// the Pipeline lays out the rendering steps for the vulkan engine to follow
class Pipeline {

    public:

    Pipeline(Device* device, Swapchain* swapchain, Logger* logger);
    ~Pipeline() = default;

    private:

    // a significant portion of nitty-gritty vulkan configuration lies in here
    bool createPipeline();

    vk::raii::Pipeline graphicsPipeline_ = nullptr;

    Device* device_ = nullptr;
    Swapchain* swapchain_ = nullptr;
    Logger* logger_ = nullptr;

};
