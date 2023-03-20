#pragma once

#include <RenderContext.h>

#include <SimpleMath.h>


class CubeRenderItem
{
public:
  CubeRenderItem(
    RenderContext* context,
    DirectX::SimpleMath::Vector3 pos = { 0.0f, 0.0f, 0.0f },
    DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f },
    DirectX::SimpleMath::Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f }
  );

  void setPosition(DirectX::SimpleMath::Vector3 pos) { mPos = pos; }
  void setScale(DirectX::SimpleMath::Vector3 scale) { mScale = scale; }
  void setColor(DirectX::SimpleMath::Vector4 color) { mColor = color; }

  bool updateSubresources();
  bool draw();

  ~CubeRenderItem();

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

  DirectX::SimpleMath::Vector3 mPos;
  DirectX::SimpleMath::Vector3 mScale;
  DirectX::SimpleMath::Vector4 mColor;
};

