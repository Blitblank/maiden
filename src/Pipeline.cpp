
#include "Pipeline.hpp"

#include "ShaderModule.hpp"

#include <array>
#include <filesystem>

Pipeline::Pipeline(Device* device, Swapchain* swapchain, Logger* logger) : device_(device), swapchain_(swapchain), logger_(logger) {
    if (device_ == nullptr || device_->logicalDevice() == nullptr ||
        *(device_->logicalDevice()) == nullptr) {

        logger_->log("Pipeline", LogFlag::Error, "Cannot create a pipeline without a valid device.");
        return;
    }

    if(!createPipeline()) {
        logger_->log("Pipeline", LogFlag::Error, "Unable to create graphics pipeline.");
    }


}

bool Pipeline::createPipeline() {
    
    // create shader stages
    const auto shaderPath = std::filesystem::path(MAIDEN_SHADER_DIR) / "triangle.spv";
    ShaderModule shaderModule(device_->logicalDevice(), shaderPath, logger_);

    const std::array shaderStages{
        shaderModule.createStageInfo(vk::ShaderStageFlagBits::eVertex, "vertMain"),
        shaderModule.createStageInfo(vk::ShaderStageFlagBits::eFragment, "fragMain"),
    };

    // dynamic pipeline state
    // allow dynamic viewport and dynamic bounds (scissors) on that viewport
    std::vector<vk::DynamicState> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor }; // giggity
    vk::PipelineDynamicStateCreateInfo dynamicStateInfo {
        .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
        .pDynamicStates = dynamicStates.data()
    }; // dynamic states are ignored by the pipeline and must be set explicitly at draw time

    // vertex input layout
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
    // empty at the moment because we're defining them all statically in the shader

    // pipeline input assembly specifies the primitive types for vulkan to construct
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo {
        .topology = vk::PrimitiveTopology::eTriangleList
    }; // other options: pointList, lineList, lineStrip, and triangleStrip

    // viewport info
    int32_t width; int32_t height;
    (void)device_->getExtent(&width, &height);
    vk::Viewport viewport {
        0.0f, // upper left corner position x
        0.0f, // upper left corner position y
        static_cast<float>(width), // width in pixels
        static_cast<float>(height), // height in pixels
        0.0f, // minDepth (from top) as a ratio of total image height
        1.0f // maxDepth (from top) as a ratio of total image height
    };
    // scissors trim the viewport rectangularly hotdog-wise
    vk::Extent2D extent = { width, height };
    vk::Rect2D scissor {
        vk::Offset2D { 0, 0 },
        extent
    };
    vk::PipelineViewportStateCreateInfo viewportStateInfo {
        .viewportCount = 1,
        .scissorCount = 1
    };

    // rasterizer info
    vk::PipelineRasterizationStateCreateInfo rasterizerInfo {
        .depthClampEnable = vk::False, // discard fragments that are outside the depth near/far planes
        .rasterizerDiscardEnable = vk::False, // keep all blank fragments from rasterization
        .polygonMode = vk::PolygonMode::eFill, // fill polygons (alternatives are Line and Point, requires gpu features)
        .cullMode = vk::CullModeFlagBits::eBack, // discard fragments that are on backs of faces
        .frontFace = vk::FrontFace::eClockwise, // consider faces that are constructed clockwise via vertex order as the front
        .depthBiasEnable = vk::False, // keep depth values raw
        .lineWidth = 1.0f // only for line mode, in units of fragments. requires enabling a gpu feature for > 1.0
    };

    // multisampling info
    vk::PipelineMultisampleStateCreateInfo multisamplingInfo {
        .rasterizationSamples = vk::SampleCountFlagBits::e1, // no multisampling here for now
        .sampleShadingEnable = vk::False
    };

    // depth stenciling
    vk::PipelineDepthStencilStateCreateInfo depthStencilInfo; // again we'll come back to this later

    // color blending
    vk::PipelineColorBlendAttachmentState colorBlendAttachment {
        .blendEnable = vk::False,
        .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        // .blendEnable = vk::True
        // .srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
        // .dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
        // .colorBlendOp        = vk::BlendOp::eAdd,
        // .srcAlphaBlendFactor = vk::BlendFactor::eOne,
        // .dstAlphaBlendFactor = vk::BlendFactor::eZero,
        // .alphaBlendOp        = vk::BlendOp::eAdd,
    }; // no color blending for now

    vk::PipelineColorBlendStateCreateInfo colorBlendingInfo {
        .logicOpEnable = vk::False,
        .logicOp = vk::LogicOp::eCopy,
        .attachmentCount = 1,
        .pAttachments = &colorBlendAttachment
    };

    // pipeline layout
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo {
        .setLayoutCount = 0,
        .pushConstantRangeCount = 0
    };
    vk::raii::PipelineLayout pipelineLayout = vk::raii::PipelineLayout(*(device_->logicalDevice()), pipelineLayoutInfo);

    // rendering pipeline
    vk::SurfaceFormatKHR* surfaceFormat = swapchain_->surfaceFormat();
    vk::GraphicsPipelineCreateInfo graphicsPipelineInfo = {
        .stageCount = 2,
        .pStages = shaderStages.data(),
        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &inputAssemblyInfo,
        .pViewportState = &viewportStateInfo,
        .pRasterizationState = &rasterizerInfo,
        .pMultisampleState = &multisamplingInfo,
        .pColorBlendState = &colorBlendingInfo,
        .pDynamicState = &dynamicStateInfo,
        .layout = pipelineLayout,
        .renderPass = nullptr
    };

    vk::PipelineRenderingCreateInfo pipelineRenderingInfo = {
        .colorAttachmentCount = 1,
        .pColorAttachmentFormats = &(surfaceFormat->format)
    };

    vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipelineCreateInfoChain = {
        graphicsPipelineInfo,
        pipelineRenderingInfo
    };

    // finally make that bad boy
    graphicsPipeline_ = vk::raii::Pipeline(*(device_->logicalDevice()), nullptr, pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>());
    if(graphicsPipeline_ == nullptr) {
        logger_->log("Pipeline", LogFlag::Error, "Unable to create graphics pipeline.");
        return false;
    } else {
        logger_->log("Pipeline", LogFlag::Info, "Created graphics pipeline.");
        return true;
    }
    // TODO: fix validation error on drawParameters that we didnt set

}
