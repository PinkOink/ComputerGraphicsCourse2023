//#define MESH 1
//#define SPHERE 1
//#define CUBE 1


#if MESH
Texture2D tex : register(t0);
SamplerState texSampler : register(s0);
#endif
Texture2D shadow : register(t1);
SamplerComparisonState shadowSampler : register(s1);

struct PixelIn
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normal : NORMAL;
#if MESH
    float2 texCoord : TEXCOORD;
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
    
    float4x4 lightViewProj;
};

cbuffer MaterialCB : register(b3)
{
#if SPHERE || CUBE
    float4 color;
#endif
    
    float3 specCoefs;
    float specParam;
}


float ShadowCalculation(float4 posLightSpace, float3 normal)
{
    float3 projPos = posLightSpace.xyz / posLightSpace.w;
    projPos.xy = projPos.xy * float2(0.5, -0.5) + float2(0.5, 0.5);
    float bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.0005);
    return shadow.SampleCmp(shadowSampler, projPos.xy, projPos.z - bias).r;
}


float4 main(PixelIn input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    
    float4 litColor;
    float3 objColor;
    
#if MESH
    float3 texVal = tex.Sample(texSampler, input.texCoord);
    
    objColor = texVal;
#elif SPHERE || CUBE
    objColor = color.xyz;
#else
    objColor = float3(1.0, 1.0, 1.0);
#endif
    
    float4 diffuseLight = float4(diffuseIntensity * objColor * max(dot(input.normal, lightDir), 0.0), 1.0);
    
    float3 viewDir = normalize(cameraPosWorld - input.posW);
    float3 reflectDir = reflect(-lightDir, input.normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specParam);
    float4 specularLight = float4(specularIntensity * specCoefs * spec, 1.0);
    
    float4 ambientLight = float4(ambientIntensity * objColor, 1.0);
    
    float shadowed = ShadowCalculation(mul(float4(input.posW, 1.0), lightViewProj), input.normal);
    
    litColor = (1.0 - shadowed) * diffuseLight + (1.0 - shadowed) * specularLight + ambientLight;
    
    return litColor;
}