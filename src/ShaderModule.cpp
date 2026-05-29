#include "ShaderModule.hpp"

#include <fstream>
#include <stdexcept>

ShaderModule::ShaderModule(vk::raii::Device* logicalDevice,
                           const std::filesystem::path& shaderPath)
    : ShaderModule(logicalDevice, loadBytecode(shaderPath)) {
}

ShaderModule::ShaderModule(vk::raii::Device* logicalDevice, std::vector<char> bytecode) {
    if (logicalDevice == nullptr || *logicalDevice == nullptr) {
        throw std::invalid_argument("Cannot create shader module with a null logical device.");
    }

    validateBytecode(bytecode);

    vk::ShaderModuleCreateInfo createInfo{
        .codeSize = bytecode.size(),
        .pCode = reinterpret_cast<const uint32_t*>(bytecode.data()),
    };

    shaderModule_ = vk::raii::ShaderModule(*logicalDevice, createInfo);
    bytecodeSize_ = bytecode.size();
}

std::vector<char> ShaderModule::loadBytecode(const std::filesystem::path& shaderPath) {
    std::ifstream file(shaderPath, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader bytecode file: " + shaderPath.string());
    }

    const std::streamsize fileSize = file.tellg();
    if (fileSize <= 0) {
        throw std::runtime_error("Shader bytecode file is empty: " + shaderPath.string());
    }

    std::vector<char> bytecode(static_cast<std::size_t>(fileSize));
    file.seekg(0, std::ios::beg);
    file.read(bytecode.data(), fileSize);

    if (!file) {
        throw std::runtime_error("Failed to read shader bytecode file: " + shaderPath.string());
    }

    validateBytecode(bytecode);
    return bytecode;
}

vk::PipelineShaderStageCreateInfo ShaderModule::createStageInfo(
    vk::ShaderStageFlagBits stage, const char* entryPoint) const {
    if (shaderModule_ == nullptr) {
        throw std::runtime_error("Cannot create shader stage info with a null shader module.");
    }

    return vk::PipelineShaderStageCreateInfo{
        .stage = stage,
        .module = *shaderModule_,
        .pName = entryPoint,
    };
}

void ShaderModule::validateBytecode(const std::vector<char>& bytecode) {
    if (bytecode.empty()) {
        throw std::runtime_error("Shader bytecode cannot be empty.");
    }

    if (bytecode.size() % sizeof(uint32_t) != 0) {
        throw std::runtime_error("Shader bytecode size must be a multiple of 4 bytes.");
    }
}
