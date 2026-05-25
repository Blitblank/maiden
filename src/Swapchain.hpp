
#pragma once

#include "Device.hpp"

// the swapchain functions as the layout for framebuffers that get written to by the gpu and read from to present to the screen
class Swapchain {

    public:

    Swapchain(Device* device);
    ~Swapchain() = default;

    std::vector<vk::Image> getImages() { return vkSwapchain_.getImages(); }

    private:

    vk::raii::SwapchainKHR vkSwapchain_ = nullptr;
    std::vector<vk::Image> swapchainImages_;

    Device* device_ = nullptr;
    vk::SurfaceFormatKHR surfaceFormat_;
    vk::PresentModeKHR presentFormat_;
    vk::Extent2D extent_;

    vk::SurfaceFormatKHR chooseSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const& availableFormats);
    vk::PresentModeKHR choosePresentMode(std::vector<vk::PresentModeKHR> const& availablePresentModes);
    vk::Extent2D chooseExtent(vk::SurfaceCapabilitiesKHR const& capabilities);
    uint32_t chooseMinImageCount(vk::SurfaceCapabilitiesKHR const& capabilities);

    bool createSwapchain();
};