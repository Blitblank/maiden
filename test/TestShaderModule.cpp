#include <gmock/gmock.h>

#include <ShaderModule.hpp>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <stdexcept>

TEST(ShaderModule, LoadsCompiledShaderBytecode) {
    ShaderModule shaderModule { nullptr, MAIDEN_TEST_SHADER_PATH, nullptr };
    const auto bytecode = shaderModule.loadBytecode(MAIDEN_TEST_SHADER_PATH);

    ASSERT_FALSE(bytecode.empty());
    EXPECT_EQ(bytecode.size() % sizeof(uint32_t), 0u);

    uint32_t magic = 0;
    std::memcpy(&magic, bytecode.data(), sizeof(magic));
    EXPECT_EQ(magic, 0x07230203u); // what
}

TEST(ShaderModule, ThrowsWhenBytecodeFileIsMissing) {
    const auto missingPath = std::filesystem::temp_directory_path() / "missing-maiden-shader.spv";

    ShaderModule shaderModule { nullptr, MAIDEN_TEST_SHADER_PATH, nullptr };
    EXPECT_THROW((void)shaderModule.loadBytecode(MAIDEN_TEST_SHADER_PATH);, std::runtime_error);
}
