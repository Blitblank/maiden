
#include "Engine.hpp"

#include <iostream>

#include "Device.hpp"
#include "Swapchain.hpp"
#include "Pipeline.hpp"

Engine::Engine(Window* window, Logger* logger): window_(window), logger_(logger) {

    // cleans up this constructor 
    init();

}

void Engine::init() {

    if(createInstance()) {
        logger_->log("Engine", LogFlag::Info, "Vulkan instance successfully created.");
    } else {
        logger_->log("Engine", LogFlag::Error, "Unable to create Vulkan instance.");
    }

    // device selection and setup
    device_ = new Device(&instance_, window_, logger_);

    // render pipeline
<<<<<<< HEAD
    swapchain_ = new Swapchain(device_, logger_);
    pipeline_ = new Pipeline(device_, swapchain_, logger_);

    (void)createSyncObjects();
    (void)createCommandBuffers();
=======
    Swapchain swapchain(&device, logger_);
    Pipeline pipeline(&device, &swapchain, logger_);
>>>>>>> c2591cf2ff7f9e3b5519dea9a635be9608c73616

}

bool Engine::drawFrame() {
    
    // wait for the last draw to complete
    if(drawFences_[frameIndex_] == nullptr) {
        logger_->log("Engine", LogFlag::Error, "drawFence_ is nullptr!");
        return false;
    }
    auto fenceResult = device_->logicalDevice()->waitForFences(*drawFences_[frameIndex_], vk::True, UINT64_MAX); // permablock until draw complete
    if(fenceResult != vk::Result::eSuccess) {
        logger_->log("Engine", LogFlag::Error, "Failed to wait for fence!");
        return false;
    }
    device_->logicalDevice()->resetFences(*drawFences_[frameIndex_]);

    // get next image index
    if(presentCompleteSemaphores_[frameIndex_] == nullptr) {
        logger_->log("Engine", LogFlag::Error, "presentCompleteSemaphore_ is nullptr!");
        return false;
    }
    vk::raii::SwapchainKHR* swapchain = swapchain_->swapchain();
    auto [semaphoreResult, imageIndex] = swapchain->acquireNextImage(UINT64_MAX, *presentCompleteSemaphores_[frameIndex_], nullptr); // again permablock
    if(semaphoreResult != vk::Result::eSuccess) {
        logger_->log("Engine", LogFlag::Error, "Failed to acquire next swapchain image!");
        return false;
    }

    // render that baby
    (void)render(imageIndex);

    // present to the screen
    const vk::PresentInfoKHR presentInfo {
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &*renderCompleteSemaphores_[frameIndex_],
        .swapchainCount = 1,
        .pSwapchains = &**swapchain, // dark programming
        .pImageIndices = &imageIndex
    };
    auto presentResult = device_->graphicsQueue().presentKHR(presentInfo);

    if(presentResult != vk::Result::eSuccess) {
        logger_->log("Engine", LogFlag::Error, "Failed to present swapchain image!");
        return false;
    }

    // advance frame
    frameIndex_ = (frameIndex_ + 1) % MAX_FRAMES_IN_FLIGHT;

    return true; // sure !~
}

bool Engine::render(uint32_t imageIndex) {

    recordCommandBuffer(imageIndex);

    // submit command buffer
    // the moment we've all been waiting for
    vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    const vk::SubmitInfo submitInfo {
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &*presentCompleteSemaphores_[frameIndex_], // &* casts a vk::raii type to just a vk:: type
        .pWaitDstStageMask = &waitDestinationStageMask,
        .commandBufferCount = 1,
        .pCommandBuffers = &*commandBuffers_[frameIndex_],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &*renderCompleteSemaphores_[frameIndex_]
    };
    device_->graphicsQueue().submit(submitInfo, *drawFences_[frameIndex_]); // :D

    device_->graphicsQueue().waitIdle();

    return true; // i guess man
}

bool Engine::createInstance() {

    uint32_t errorCount = 0;

    // create the appInfo filled with information about our app
    constexpr vk::ApplicationInfo appInfo { // using the c++ api instead of the c api
        .pApplicationName = "maiden",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "null",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_3 // this one is most important
    };

    std::vector<const char*> requiredInstanceExtensions = getRequiredInstanceExtensions();

    // get all available extensions
    auto extensionProperties = context_.enumerateInstanceExtensionProperties();

    // print if we feel like it
    logger_->log("Engine", LogFlag::Debug, "Available Vulkan Extensions:");
    for(const auto& extensionProperty : extensionProperties) {
        // TODO: log function probably could do with variable arguments and string formatting because this is slightly clunky
        std::string extensionName = (extensionProperty.extensionName).data();
        std::string msg = "\t" + extensionName;
        logger_->log("Engine", LogFlag::Debug, msg);
    }

    // check that all required extensions are available
    for(uint32_t i = 0; i < requiredInstanceExtensions.size(); i++) { // for each extension that we require
        bool found = false;
        for(const auto& extensionProperty : extensionProperties) { // see if it matches any extensions that are provided
            if(strcmp(extensionProperty.extensionName, requiredInstanceExtensions[i]) == 0) {
                found = true;
                break;
            }
        }
	    if(!found) {
            std::string msg = "Required SDL3 extension not supported: " + std::string(requiredInstanceExtensions[i]);
            logger_->log("Engine", LogFlag::Warning, msg);
            errorCount++;
        } else {
	        // in case you're curious
            std::string msg = "SDL3 extension located: " + std::string(requiredInstanceExtensions[i]);
            logger_->log("Engine", LogFlag::Debug, msg);
        }
    }

    // get required validation layers as specified by our app
    std::vector<const char*> requiredValidationLayers;
    if(enableValidationLayers) requiredValidationLayers.assign(validationLayers.begin(), validationLayers.end());

    // get available validation layers
    auto validationLayerProperties = context_.enumerateInstanceLayerProperties();

    // again print if we feel like it
    logger_->log("Engine", LogFlag::Debug, "Available Vulkan Validation Layers:");
    for(const auto& validationLayer : validationLayerProperties) {
        std::string layerName = (validationLayer.layerName).data();
        std::string msg = "\t" + layerName;
        logger_->log("Engine", LogFlag::Debug, msg);
    }

    // check that all required validation layers are avilable
    for(int i = 0; i < requiredValidationLayers.size(); i++) {
        bool found = false;
        for(const auto& validationLayer : validationLayerProperties) {
            if(strcmp(requiredValidationLayers[i], validationLayer.layerName) == 0) {
                found = true;
                break;
            }
        }
        if(!found) {
            errorCount++;
            std::string msg = "Required validation layer not supported: " + std::string(requiredValidationLayers[i]);
            logger_->log("Engine", LogFlag::Warning, msg);
        } else { // in case you're curious
            std::string msg = "VkValidation layer located: " + std::string(requiredValidationLayers[i]);
            logger_->log("Engine", LogFlag::Debug, msg);
        }
    }
    
    // if any we had errors then we must exit
    if(errorCount != 0) {
        std::string msg = "Unable to create Vulkan instance. Error count: " + std::to_string(errorCount);
        logger_->log("Engine", LogFlag::Debug, msg);
        return false;
    }

    vk::InstanceCreateInfo instanceCreateInfo {
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = static_cast<uint32_t>(requiredValidationLayers.size()),
        .ppEnabledLayerNames = requiredValidationLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(requiredInstanceExtensions.size()),
        .ppEnabledExtensionNames = requiredInstanceExtensions.data()
    };

    instance_ = vk::raii::Instance(context_, instanceCreateInfo);
    return (instance_ != nullptr);
}

std::vector<const char*> Engine::getRequiredInstanceExtensions() {

    // get extensions that our windowing library requires
    uint32_t sdlExtensionsCount = 0;
    const char* const* sdlExtensions{ SDL_Vulkan_GetInstanceExtensions(&sdlExtensionsCount) }; // TODO: get this from window so all sdl3 is encapsulated there
    // what in the world is this kind of pointer btw

    std::vector<const char*> requiredExtensions(sdlExtensions, sdlExtensions + static_cast<size_t>(sdlExtensionsCount));

    // manually add an extension for handling validation layers
    if(enableValidationLayers) {
        requiredExtensions.push_back(vk::EXTDebugUtilsExtensionName);
    }

    return requiredExtensions;
}

bool Engine::initDebugMessenger() {

    if(!enableValidationLayers) return false;

    // masks for which debug messages we want to see
    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
    vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{.messageSeverity = severityFlags,
                                                                          .messageType     = messageTypeFlags,
                                                                          .pfnUserCallback = &debugCallback};
    debugMessenger_ = instance_.createDebugUtilsMessengerEXT( debugUtilsMessengerCreateInfoEXT );
    // we could get rid of this and just pass all the control to the logger
    // like: "treat all messages of severity vk::eVerbose as our own DebugVerbosity::Info"

    return (debugMessenger_ != nullptr);
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL Engine::debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                              vk::DebugUtilsMessageTypeFlagsEXT type,
                                                              const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                              void* pUserData) {

    // validation layers are for debugging only (and also its a pain hooking a nonstatic object to a static function)
    std::cout << "[ Validation Layer ] [Type: " << to_string(type) << "] " <<  pCallbackData->pMessage << std::endl;
    /*
    vk severity types:
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eError

    vk message types:
    vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
    vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
    vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
    */

    // returns whether or not we should abort, we'll always say no
    return vk::False;
}

bool Engine::createCommandBuffers() {

    vk::CommandBufferAllocateInfo allocInfo {
        .commandPool = *(device_->commandPool()),
        .level = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = MAX_FRAMES_IN_FLIGHT
    };

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        commandBuffers_ = vk::raii::CommandBuffers(*(device_->logicalDevice()), allocInfo);
    }

    return true;

}

bool Engine::recordCommandBuffer(int frameIndex) {

    commandBuffers_[frameIndex_].begin({});

    transitionImageLayout(frameIndex, vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal, {},
        vk::AccessFlagBits2::eColorAttachmentWrite, vk::PipelineStageFlagBits2::eTopOfPipe, vk::PipelineStageFlagBits2::eColorAttachmentOutput);

    // command for clearing the framebuffer
    vk::ClearValue clearColor = vk::ClearColorValue(0.01f, 0.01f, 0.02f, 1.0f);
    vk::RenderingAttachmentInfo attachmentInfo = {
        .imageView = swapchain_->imageView(frameIndex),
        .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
        .loadOp = vk::AttachmentLoadOp::eClear,
        .storeOp = vk::AttachmentStoreOp::eStore,
        .clearValue = clearColor
    };

    // command to begin rendering
    vk::RenderingInfo renderingInfo = {
        .renderArea = { .offset = { 0, 0 }, .extent = swapchain_->extent() }, // render to the full extent
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &attachmentInfo
    };
    commandBuffers_[frameIndex_].beginRendering(renderingInfo);

    // attach pipeline
    commandBuffers_[frameIndex_].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline_->pipeline());

    // set rendering range
    commandBuffers_[frameIndex_].setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(swapchain_->extent().width), static_cast<float>(swapchain_->extent().height), 0.0f, 1.0f));
    commandBuffers_[frameIndex_].setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), swapchain_->extent()));

    // command to draw geometry and fragments
    commandBuffers_[frameIndex_].draw(3, 1, 0, 0); // vertexCount, instanceCount, firstVertex offset, firstInstance offset

    // end draw call
    commandBuffers_[frameIndex_].endRendering();

    // commands present to screen
    transitionImageLayout(frameIndex, vk::ImageLayout::eColorAttachmentOptimal, vk::ImageLayout::ePresentSrcKHR, vk::AccessFlagBits2::eColorAttachmentWrite,
        {}, vk::PipelineStageFlagBits2::eColorAttachmentOutput, vk::PipelineStageFlagBits2::eBottomOfPipe);

    commandBuffers_[frameIndex_].end();

    return true;

}

// TODO: maybe this should be in swapchain, like above call commandBuffer.pipelineBarrier2(transitionImageLayout...);
void Engine::transitionImageLayout(uint32_t imageIndex, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::AccessFlags2 srcAccessMask,
    vk::AccessFlags2 destAccessMask, vk::PipelineStageFlags2 srcStageMask, vk::PipelineStageFlags2 destStageMask) {

        vk::ImageMemoryBarrier2 barrier = {
            .srcStageMask = srcStageMask,
            .srcAccessMask = srcAccessMask,
            .dstStageMask = destStageMask,
            .dstAccessMask = destAccessMask,
            .oldLayout = oldLayout,
            .newLayout = newLayout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = swapchain_->image(imageIndex),
            .subresourceRange = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        vk::DependencyInfo dependencyInfo = {
            .dependencyFlags = {},
            .imageMemoryBarrierCount = 1,
            .pImageMemoryBarriers = &barrier
        };

        commandBuffers_[frameIndex_].pipelineBarrier2(dependencyInfo);
}

bool Engine::createSyncObjects() {

    if(!presentCompleteSemaphores_.empty() && renderCompleteSemaphores_.empty() && drawFences_.empty()) {
        logger_->log("Engine", LogFlag::Error, "Synchronization objects already exist!");
        return false;
    }

    for(size_t i = 0; i < swapchain_->getImages().size(); i++) {
        
        renderCompleteSemaphores_.emplace_back(vk::raii::Semaphore(*(device_->logicalDevice()), vk::SemaphoreCreateInfo()));
    }

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        presentCompleteSemaphores_.emplace_back(vk::raii::Semaphore(*(device_->logicalDevice()), vk::SemaphoreCreateInfo()));
        drawFences_.emplace_back(*(device_->logicalDevice()), vk::FenceCreateInfo{ .flags = vk::FenceCreateFlagBits::eSignaled });
    }

    return true;
}
