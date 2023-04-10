
struct VertexIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};


cbuffer SceneCB : register(b0)
{
    float4x4 viewProj;
    
    float3 cameraPosWorld;
};

cbuffer MeshCB : register(b1)
{
    float4x4 world;
    float4x4 worldInv;
};


VertexOut main(VertexIn input)
{
    VertexOut output;
    
    output.posW = mul(float4(input.pos, 1.0), world);
    output.posH = mul(float4(output.posW, 1.0), viewProj);
    output.normal = normalize(mul(float4(input.normal, 0.0), world).xyz);
    output.texCoord = input.texCoord;
    
    return output;
}