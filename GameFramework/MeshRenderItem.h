#pragma once

#include "RenderContext.h"

#include <SimpleMath.h>

#include "RenderItem.h"


struct MeshMaterial
{
  DirectX::SimpleMath::Vector3 specCoefs = { 1.0f, 1.0f, 1.0f };
  float specParam = 1.0;
};


class MeshRenderItem : public RenderItem
{
public:
  MeshRenderItem(
    RenderContext* context,
    const std::string& meshFilename,
    const std::wstring& textureFilename,
    const std::wstring& vertexShaderFilename,
    const std::wstring& pixelShaderFilename,
    MeshMaterial material = {},
    DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f },
    DirectX::SimpleMath::Vector3 posOffset = { 0.0f, 0.0f, 0.0f }
  );

  virtual void setWorldMatrix(const DirectX::SimpleMath::Matrix& mat) override;

  virtual bool updateSubresources() override;
  virtual bool draw() override;

  virtual bool drawGeometry() override;

  virtual ~MeshRenderItem() override;

protected:
  RenderContext* mContext;

  D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRastState = nullptr;
  Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState = nullptr;

  Microsoft::WRL::ComPtr<ID3D11InputLayout> mLayout = nullptr;

  Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader = nullptr;
  Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader = nullptr;

  Microsoft::WRL::ComPtr<ID3D11Buffer> mPositionsBuffer = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Buffer> mNormalsBuffer = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Buffer> mTexCoordsBuffer = nullptr;

  int mVerticesNum = 0;

  Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Buffer> mMaterialCB = nullptr;

  Microsoft::WRL::ComPtr<ID3D11Resource> mTexture = nullptr;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTextureView = nullptr;

  DirectX::SimpleMath::Vector3 mScale;

  DirectX::SimpleMath::Matrix mWorldMat;
};

