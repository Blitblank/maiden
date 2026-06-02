
#include "Swapchain.hpp"

#include <iostream>

Swapchain::Swapchain(Device* device, Logger* logger) : device_(device), logger_(logger) {

    (void)createSwapchain();
    (void)createImageViews();

}

bool Swapchain::createSwapchain() {

    // get capabilities from the device
    vk::raii::PhysicalDevice physicalDevice = device_->physicalDevice();
    vk::raii::Device* logicalDevice = device_->logicalDevice();
    vk::raii::SurfaceKHR* surface = device_->getSurface();
    vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
    extent_ = chooseExtent(surfaceCapabilities);
    uint32_t minImageCount = chooseMinImageCount(surfaceCapabilities);

    std::vector<vk::SurfaceFormatKHR> availableFormats = physicalDevice.getSurfaceFormatsKHR(*surface);
    surfaceFormat_ = chooseSurfaceFormat(availableFormats);

    std::vector<vk::PresentModeKHR> availablePresentModes = physicalDevice.getSurfacePresentModesKHR(*surface);
    vk::PresentModeKHR presentMode = choosePresentMode(availablePresentModes);

    // create the swapchain object
    vk::SwapchainCreateInfoKHR swapchainCreateInfo {
        .surface = *surface,
        .minImageCount = minImageCount,
        .imageFormat = surfaceFormat_.format,
        .imageColorSpace = surfaceFormat_.colorSpace,
        .imageExtent = extent_,
        .imageArrayLayers = 1, // only rendering to a single screen
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment, // draw directly to swapchain image (as opposed to an offscreen image)
        .imageSharingMode = vk::SharingMode::eExclusive, // owned only by a single queue
        .preTransform = surfaceCapabilities.currentTransform, // dont flip or rotate the image
        .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque, // use other settings if you want the window to be partly transparent :o
        .presentMode = presentMode,
        .clipped = true // discard fragments in the window that aren't visible (obscured by another window or minimized)
    };
    // TODO: [after presentation] need to handle swapchain recreation as a result of window changes
    swapchainCreateInfo.oldSwapchain = nullptr;

    vkSwapchain_ = vk::raii::SwapchainKHR(*logicalDevice, swapchainCreateInfo);
    images_ = vkSwapchain_.getImages();

    if(vkSwapchain_ == nullptr) {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: could not create the Vulkan Swapchain!" << std::endl;
        return false;
    } else {
        return true;
    }
}

vk::SurfaceFormatKHR Swapchain::chooseSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const& availableFormats) {

    if(availableFormats.empty()) {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: no swap formats available!" << std::endl;
        return vk::SurfaceFormatKHR { vk::Format::eUndefined, vk::ColorSpaceKHR::eSrgbNonlinear };
    }

    // check for preferred format vk::Format::eB8G8R8A8Srgb
    const auto formatIt = std::ranges::find_if(availableFormats, [](const auto& format) {
        return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
    });

    // just return whatever if the correct one doesn't exist
    return formatIt != availableFormats.end() ? *formatIt : availableFormats[0];

}

vk::PresentModeKHR Swapchain::choosePresentMode(std::vector<vk::PresentModeKHR> const& availablePresentModes) {

    // vk::PresentModeKHR::eImmediate: present rendered fragments directly to the surface (results in frame tearing)
    // vk::PresentModeKHR::eFifo: first in first out, waits for frame refresh (vsync)
    // vk::PresentModeKHR::eFifoRelaxed: same as fifo but does not wait for vsync if the last frame was late
    // vk::PresentModeKHR::eMailbox: same as fifo but continues rendering new frames while waiting for vsync

    if(availablePresentModes.empty()) {
        // fifo is guaranteed to be available so this is unreachable
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: no swap formats available!" << std::endl;
        return vk::PresentModeKHR::eFifo;
    }

    // prefer mailbox if it exists; mailbox ensures least latency between displayed frames and cpu commands
    assert(std::ranges::any_of(availablePresentModes, [](auto presentMode) { return presentMode == vk::PresentModeKHR::eFifo; }));
    return std::ranges::any_of(availablePresentModes, [](const vk::PresentModeKHR value) {
        return vk::PresentModeKHR::eMailbox == value; }) ? vk::PresentModeKHR::eMailbox : vk::PresentModeKHR::eFifo;

}

vk::Extent2D Swapchain::chooseExtent(vk::SurfaceCapabilitiesKHR const& capabilities) {

    if(capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }

    // otherwise we have to get the swap extent via the window
    int32_t width = 0;
    int32_t height = 0;
    (void)device_->getExtent(&width, &height);
    vk::Extent2D extent = {
        std::clamp<uint32_t>(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
        std::clamp<uint32_t>(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
    };
    return extent;
}

uint32_t Swapchain::chooseMinImageCount(vk::SurfaceCapabilitiesKHR const& capabilities) {

    // prefer at least 3 images for the swapchain, otherwise default to the max the driver can support
    uint32_t minImageCount = std::max(3u, capabilities.minImageCount); // TODO: magic numbers ?
    if((0 < capabilities.maxImageCount) && capabilities.maxImageCount < minImageCount) {
        return capabilities.maxImageCount;
    } else {
        return minImageCount;
    }
}

bool Swapchain::createImageViews() {

    if(!imageViews_.empty()) {
        return false;
    }

    vk::ImageViewCreateInfo imageViewCreateInfo {
        .viewType = vk::ImageViewType::e2D,
        .format = surfaceFormat_.format,
        .components = { vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity }, // default rgba
        .subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}, // aspect mask, baseMipLevel, levelCount, baseArrayLayer, layerCount
    };

    for(vk::Image &image : images_) {
        imageViewCreateInfo.image = image;
        imageViews_.emplace_back(*(device_->logicalDevice()), imageViewCreateInfo);
    }

    return true;
}
