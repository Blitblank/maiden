
#pragma once

#include "Device.hpp"

// the Pipeline lays out the rendering steps for the vulkan engine to follow
class Pipeline {

    public:

    Pipeline(Device* device);
    ~Pipeline() = default;

    private:

    Device* device_ = nullptr;

    // will include shaders eventually
};
