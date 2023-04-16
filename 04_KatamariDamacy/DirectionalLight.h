#pragma once

#include <GameComponent.h>
#include <RenderContext.h>
#include <SimpleMath.h>

#include "PlayerComponent.h"


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

  DirectX::SimpleMath::Matrix viewProj;
};


class DirectionalLight : public GameComponent
{
public:
  DirectionalLight(RenderContext* context, PlayerComponent* player, DirectionalLightCB lightData = {});

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool updateSubresources();
  virtual bool draw();

  void setGeometries(std::vector<RenderItem*>& geometry) { mGeometry = geometry; }

  virtual ~DirectionalLight();

protected:
  RenderContext* mContext;
  PlayerComponent* mPlayer;
  std::vector<RenderItem*> mGeometry;

  const unsigned int mShadowMapSize = 8192;

  const float mRotateSpeed = 3.0f;

  DirectionalLightCB mLightData;;
  
  Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Buffer> mShadowCB = nullptr;

  D3D11_VIEWPORT mShadowViewport = {};
  Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mShadowState = nullptr;
  Microsoft::WRL::ComPtr<ID3D11VertexShader> mShadowVS = nullptr;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> mShadowLayout = nullptr;

  Microsoft::WRL::ComPtr<ID3D11Texture2D> mShadowTex = nullptr;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mShadowTargetView = nullptr;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShadowResourceView = nullptr;
  Microsoft::WRL::ComPtr<ID3D11SamplerState> mShadowSamplerState = nullptr;
};

