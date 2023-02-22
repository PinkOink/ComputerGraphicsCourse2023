
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


VertexOut main(VertexIn input)
{
    VertexOut output;
    
    output.pos = input.pos;
    output.col = input.col;
    
    return output;
}