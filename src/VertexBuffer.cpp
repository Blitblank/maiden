#include "Logger.hpp"
#include "VertexBuffer.hpp"
#include <cstring> // for memcpy


VertexBuffer::VertexBuffer(Logger& logs, Device& device, Mesh& mesh) : loggerref(logs), deviceref(device), meshref(mesh)
{

}

void VertexBuffer::CreateBuffer()
{

    std::vector<Vertex> vertices = meshref.getallvertices(); // Getting from the array of Vertices 

    vk::raii::Device* logicalDevice = deviceref.logicalDevice(); // Get logical device info from the created device object.


    // Creating a buffer info struct to hold buffer info for buffer creation.
    bufferInfo = vk::BufferCreateInfo{.size  = sizeof(Vertex) * vertices.size(), 
    .sharingMode = sharingMode};
    vertexBuffer = vk::raii::Buffer(*logicalDevice, bufferInfo); 
    
    vk::MemoryRequirements memRequirements = vertexBuffer.getMemoryRequirements();

    vk::MemoryAllocateInfo memoryAllocateInfo{.allocationSize  = memRequirements.size, .memoryTypeIndex = getmemorytype(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)};

    bufferMemory = vk::raii::DeviceMemory(*logicalDevice, memoryAllocateInfo);

    vertexBuffer.bindMemory(*bufferMemory, 0);

    loggerref.log("VertexBuffer", LogFlag::Debug, "Vertex buffer created and memory reserved");


    // Filling the vertex buffer.
    void* data = bufferMemory.mapMemory(0, bufferInfo.size);

    memcpy(data, vertices.data(), static_cast<size_t>(bufferInfo.size));

    bufferMemory.unmapMemory();

}


uint32_t VertexBuffer::getmemorytype( uint32_t typeFilter, vk::MemoryPropertyFlags properties) // returns  unsinged 32 bit integer for vk Memory allocate info
{

    uint32_t typeindex = 0;
    bool found = false;

    vk::raii::PhysicalDevice physicalDevice = deviceref.physicalDevice();
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

    for (  uint32_t index = 0; index < memProperties.memoryTypeCount; index++)

    {
        if ((typeFilter) & (1 << index))
        {
            if ((memProperties.memoryTypes[index].propertyFlags & properties) == properties)
            {
            loggerref.log("VertexBuffer", LogFlag::Debug, "Found a compatible memory type at Index " + std::to_string(index)  );
            typeindex = index;
            found = true;
            return typeindex;
            }
        }
    }

    if (!found)
    {
    loggerref.log("VertexBuffer", LogFlag::Error, "Unable to find a compatible memory type");
        //error handling here
    }
}






