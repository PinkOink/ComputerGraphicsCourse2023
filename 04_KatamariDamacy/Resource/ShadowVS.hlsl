
cbuffer LightCB : register(b0)
{
    float4x4 viewProj;
};

cbuffer MeshCB : register(b1)
{
    float4x4 world;
    float4x4 worldInv;
};


float4 main( float3 pos : POSITION ) : SV_POSITION
{
    return mul(mul(float4(pos, 1.0), world), viewProj);
}