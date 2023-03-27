
Texture2D tex : register(t0);
SamplerState texSampler : register(s0);

struct PixelIn
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

float4 main(PixelIn input) : SV_TARGET
{
    return tex.Sample(texSampler, input.texCoord);
}