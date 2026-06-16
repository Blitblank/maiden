static float2 positions[3] = float2[](
    float2(0.75, 0.75),
    float2(-0.75, 0.75),
    float2(0.0, -0.75)
);

static float3 colors[3] = float3[](
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
);

struct VSInput {
    float2 inPosition;
    float3 inColor;
};

struct UniformBuffer {
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};
ConstantBuffer<UniformBuffer> ubo;

struct VSOutput {
    float4 pos : SV_Position;
    float3 color;
};

struct VertexOutput {
    float3 color : COLOR0;
    float4 position : SV_Position;
};

[shader("vertex")]
VSOutput vertMain(VSInput input) {
    VSOutput output;
    output.pos = mul(ubo.proj, mul(ubo.view, mul(ubo.model, float4(input.inPosition, 0.0, 1.0))));
    output.color = input.inColor;
    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput vertIn) : SV_Target {
    return float4(vertIn.color, 1.0);
}
