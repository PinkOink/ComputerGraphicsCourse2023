
struct VertexIn
{
    float4 pos : POSITION0;
    float4 col : COLOR0;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

cbuffer SquareCB : register(b0)
{
    float4x4 transform;
};


VertexOut main(VertexIn input)
{
    VertexOut output;
    
    output.pos = mul(input.pos, transform);
    output.col = input.col;
    
    return output;
}