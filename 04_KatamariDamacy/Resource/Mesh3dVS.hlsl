
struct VertexIn
{
    float4 pos : POSITION;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

cbuffer SceneCB : register(b0)
{
    float4x4 viewProj;
};

cbuffer MeshCB : register(b1)
{
    float4x4 transform;
    float4 color;
};


VertexOut main(VertexIn input)
{
    VertexOut output;
    
    output.pos = mul(mul(input.pos, transform), viewProj);
    output.col = color;
    
    return output;
}