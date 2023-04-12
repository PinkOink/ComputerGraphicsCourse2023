#pragma once

#include <GameComponent.h>
#include <RenderContext.h>
#include <SimpleMath.h>


struct DirectionalLightCB
{
  DirectX::SimpleMath::Vector3 diffuseIntensity = { 0.6f, 0.6f, 0.6f };
  float pad1;
  DirectX::SimpleMath::Vector3 specularIntensity = { 1.0f, 1.0f, 1.0f };
  float pad2;
  DirectX::SimpleMath::Vector3 ambientIntensity = { 0.2f, 0.2f, 0.2f };
  float pad3;

  DirectX::SimpleMath::Vector3 lightDir = { 0.0f, 0.0f, 1.0f };
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

  const float mRotateSpeed = 3.0f;

  DirectionalLightCB mLightData;;
  
  Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;
};

