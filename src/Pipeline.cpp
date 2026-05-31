
#include "Pipeline.hpp"

#include "ShaderModule.hpp"

#include <array>
#include <filesystem>

Pipeline::Pipeline(Device* device, Logger* logger) : device_(device), logger_(logger) {
    if (device_ == nullptr || device_->logicalDevice() == nullptr ||
        *(device_->logicalDevice()) == nullptr) {
        return;
    }

    const auto shaderPath = std::filesystem::path(MAIDEN_SHADER_DIR) / "triangle.spv";
    ShaderModule shaderModule(device_->logicalDevice(), shaderPath, logger_);

    const std::array shaderStages{
        shaderModule.createStageInfo(vk::ShaderStageFlagBits::eVertex, "vertMain"),
        shaderModule.createStageInfo(vk::ShaderStageFlagBits::eFragment, "fragMain"),
    };
    (void)shaderStages;
}
