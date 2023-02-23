
struct VertexIn
{
    float4 pos : POSITION;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

cbuffer SquareCB : register(b0)
{
    float4x4 transform;
    float4 color;
};


VertexOut main(VertexIn input)
{
    VertexOut output;
    
    output.pos = mul(input.pos, transform);
    output.col = color;
    
    return output;
}