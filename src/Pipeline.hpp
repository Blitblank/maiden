
#pragma once

#include "glm/glm.hpp"

#include "Device.hpp"
#include "Swapchain.hpp"

struct UniformBufferObject {
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;  
};

// the Pipeline lays out the rendering steps for the vulkan engine to follow
class Pipeline {

    public:

    Pipeline(Device* device, Swapchain* swapchain, Logger* logger);
    ~Pipeline() = default;

    vk::Pipeline pipeline() { return *graphicsPipeline_; }

    private:

    // a significant portion of nitty-gritty vulkan configuration lies in here
    bool createPipeline();

    // main render pass prerequisites (unused)
    void createRenderPass();
    void createDescriptorSetLayout();

    vk::raii::Pipeline graphicsPipeline_ = nullptr;
    vk::raii::PipelineLayout pipelineLayout_ = nullptr;
    vk::raii::DescriptorSetLayout descriptorSetLayout_ = nullptr;

    Device* device_ = nullptr;
    Swapchain* swapchain_ = nullptr;
    Logger* logger_ = nullptr;

};
