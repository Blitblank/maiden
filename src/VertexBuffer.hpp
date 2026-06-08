#pragma once
#include <vector>
#include "Logger.hpp"
#include "Mesh.hpp"
#include <vulkan/vulkan_raii.hpp>
#include "Device.hpp"

class VertexBuffer
{

public:
VertexBuffer(Logger& logs, Device& device, Mesh& mesh);


~VertexBuffer() =default;

void CreateBuffer();


uint32_t getmemorytype(uint32_t typeFilter, vk::MemoryPropertyFlags properties);


private:

Logger& loggerref;

Device& deviceref;

Mesh& meshref;

vk::BufferCreateInfo bufferInfo;

vk::raii::Buffer vertexBuffer = nullptr;

vk::raii::DeviceMemory bufferMemory = nullptr;

/*
vk::raii::Device device = nullptr; // Get this from device object


vk::raii::Device* logicalDevice = deviceRef.logicalDevice();

vk::raii::PhysicalDevice physicalDevice = deviceRef.physicalDevice();
*/


// 
// Configs for Buffer info

vk::BufferUsageFlagBits usage = vk::BufferUsageFlagBits::eVertexBuffer; 
vk::SharingMode sharingMode = vk::SharingMode::eExclusive;

//
// Configs For Memory Requirements


};

