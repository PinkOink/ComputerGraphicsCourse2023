
struct PixelIn
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};


float4 main(PixelIn input) : SV_TARGET
{
    return input.color;
}