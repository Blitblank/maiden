#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <vulkan/vulkan_raii.hpp>

class ShaderModule {
public:
    ShaderModule(vk::raii::Device* logicalDevice, const std::filesystem::path& shaderPath);
    ShaderModule(vk::raii::Device* logicalDevice, std::vector<char> bytecode);
    ~ShaderModule() = default;

    static std::vector<char> loadBytecode(const std::filesystem::path& shaderPath);

    vk::PipelineShaderStageCreateInfo createStageInfo(vk::ShaderStageFlagBits stage,
                                                      const char* entryPoint) const;
    std::size_t bytecodeSize() const { return bytecodeSize_; }

private:
    static void validateBytecode(const std::vector<char>& bytecode);

    vk::raii::ShaderModule shaderModule_ = nullptr;
    std::size_t bytecodeSize_ = 0;
};
