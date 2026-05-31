
#pragma once

#include "Device.hpp"

// the Pipeline lays out the rendering steps for the vulkan engine to follow
class Pipeline {

    public:

    Pipeline(Device* device, Logger* logger);
    ~Pipeline() = default;

    private:

    // a significant portion of nitty-gritty vulkan configuration lies in here
    bool createPipeline();

    vk::raii::PipelineLayout pipelineLayout_ = nullptr;

    Device* device_ = nullptr;
    Logger* logger_ = nullptr;

};
