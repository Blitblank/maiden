
#pragma once

// resource allocation is initializion my beloved
#include <vulkan/vulkan_raii.hpp>

#include "Window.hpp"

class Engine {

public:

    Engine(Window* window);
    ~Engine() = default;

    void init();
    
    // draw is called every render iteration in that while loop
    void draw();

private:

    Window* window_;

    bool createInstance();

    // Vulkan specific
    vk::raii::Context context_;
    vk::raii::Instance instance_ = nullptr;

};
