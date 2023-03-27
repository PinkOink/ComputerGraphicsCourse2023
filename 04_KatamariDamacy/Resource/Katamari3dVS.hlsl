
struct VertexIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

cbuffer SceneCB : register(b0)
{
    float4x4 viewProj;
};

cbuffer MeshCB : register(b1)
{
    float4x4 transform;
};


VertexOut main(VertexIn input)
{
    VertexOut output;
    
    output.pos = mul(mul(float4(input.pos, 1.0), transform), viewProj);
    output.texCoord = input.texCoord;
    
    return output;
}