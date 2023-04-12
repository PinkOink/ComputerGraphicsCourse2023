#pragma once

#include "RenderContext.h"

#include <SimpleMath.h>

#include "RenderItem.h"


struct CubeMaterial
{
  DirectX::SimpleMath::Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

  DirectX::SimpleMath::Vector3 specCoefs = { 1.0f, 1.0f, 1.0f };
  float specParam = 1.0;
};


class CubeRenderItem : public RenderItem
{
public:
  CubeRenderItem(
    RenderContext* context,
    DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f },
    CubeMaterial material = {},
    const std::wstring& vertexShaderFilename = L"./Resource/Mesh3dVS.hlsl",
    const std::wstring& pixelShaderFilename = L"./Resource/Mesh3dPS.hlsl"
  );

  virtual void setWorldMatrix(const DirectX::SimpleMath::Matrix& mat) override;

  virtual bool updateSubresources() override;
  virtual bool draw() override;

  virtual ~CubeRenderItem() override;

protected:
  RenderContext* mContext;

  D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRastState = nullptr;

  Microsoft::WRL::ComPtr<ID3D11InputLayout> mLayout = nullptr;

  Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader = nullptr;
  Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader = nullptr;

  Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer = nullptr;

  Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Buffer> mMaterialCB = nullptr;

  DirectX::SimpleMath::Vector3 mScale;

  DirectX::SimpleMath::Matrix mWorldMat;
};

