
Texture2D tex : register(t0);
SamplerState texSampler : register(s0);

struct PixelIn
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};


cbuffer DirectionalLightCB : register(b2)
{
    float3 diffuseIntensity;
    float3 specularIntensity;
    float3 ambientIntensity;

    float3 lightDir;
};


float4 main(PixelIn input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    
    float3 texVal = tex.Sample(texSampler, input.texCoord);
    
    float4 litColor = float4(texVal, 1.0);
    
    return litColor;
}