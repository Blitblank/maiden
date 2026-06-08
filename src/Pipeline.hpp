
#pragma once

#include "Device.hpp"
#include "Swapchain.hpp"

// the Pipeline lays out the rendering steps for the vulkan engine to follow
class Pipeline {

    public:

    Pipeline(Device* device, Swapchain* swapchain, Logger* logger);
    ~Pipeline() = default;

    vk::Pipeline pipeline() { return *graphicsPipeline_; }

    private:

    // a significant portion of nitty-gritty vulkan configuration lies in here
    bool createPipeline();

<<<<<<< HEAD
    // main render pass prerequisites (unused)
    void createRenderPass();

=======
>>>>>>> c2591cf2ff7f9e3b5519dea9a635be9608c73616
    vk::raii::Pipeline graphicsPipeline_ = nullptr;

    Device* device_ = nullptr;
    Swapchain* swapchain_ = nullptr;
    Logger* logger_ = nullptr;

};
