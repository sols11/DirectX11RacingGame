// 添加光照需要的ConstantBuffer（删掉了alpha因为一个shader只能有一个buffer）
cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    // 光照与纹理颜色混合
    float3 ambientLight = ambientLightColor * ambientLightStrength;
    float3 finalColor = sampleColor * ambientLight;
    return float4(finalColor, 1.0f);
}