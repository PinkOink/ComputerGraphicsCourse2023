#pragma once

#include <GameComponent.h>
#include <RenderContext.h>
#include <SimpleMath.h>


struct DirectionalLightCB
{
  DirectX::SimpleMath::Vector3 diffuseIntensity = { 1.0, 1.0, 1.0 };
  float pad1;
  DirectX::SimpleMath::Vector3 specularIntensity = { 1.0, 1.0, 1.0 };
  float pad2;
  DirectX::SimpleMath::Vector3 ambientIntensity = { 0.2, 0.2, 0.2 };
  float pad3;

  DirectX::SimpleMath::Vector3 lightDir = { 0.0, 0.0, 1.0 };
  float pad4;
};


class DirectionalLight : public GameComponent
{
public:
  DirectionalLight(RenderContext* context, DirectionalLightCB lightData = {});

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool updateSubresources();
  virtual bool draw();

  virtual ~DirectionalLight();

protected:
  RenderContext* mContext;

  DirectionalLightCB mLightData;;
  
  Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;
};

