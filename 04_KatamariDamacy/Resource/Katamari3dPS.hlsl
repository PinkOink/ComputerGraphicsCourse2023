//#define MESH 1
//#define SPHERE 1
//#define CUBE 1


#if MESH
Texture2D tex : register(t0);
SamplerState texSampler : register(s0);
#endif

struct PixelIn
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normal : NORMAL;
#if MESH
    float2 texCoord : TEXCOORD;
#elif SPHERE || CUBE
    float4 color : COLOR;
#endif
};


cbuffer SceneCB : register(b0)
{
    float4x4 viewProj;
    
    float3 cameraPosWorld;
};

cbuffer DirectionalLightCB : register(b2)
{
    float3 diffuseIntensity;
    float pad1;
    float3 specularIntensity;
    float pad2;
    float3 ambientIntensity;
    float pad3;

    float3 lightDir;
    float pad4;
};


float4 main(PixelIn input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    
    float4 litColor;
    float3 objColor;
    
#if MESH
    float3 texVal = tex.Sample(texSampler, input.texCoord);
    
    objColor = texVal;
#elif SPHERE || CUBE
    objColor = input.color.xyz;
#else
    objColor = float3(1.0, 1.0, 1.0);
#endif
    
    float4 diffuseLight = float4(diffuseIntensity * objColor * max(dot(input.normal, lightDir), 0.0), 1.0);
    
    float3 viewDir = normalize(cameraPosWorld - input.posW);
    float3 reflectDir = reflect(-lightDir, input.normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float4 specularLight = float4(specularIntensity * objColor * spec, 1.0);
    
    float4 ambientLight = float4(ambientIntensity * objColor, 1.0);
    
    litColor = diffuseLight + specularLight + ambientLight;
    
    return litColor;
}