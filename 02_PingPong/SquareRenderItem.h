#pragma once

#include <GameComponent.h>
#include <RenderContext.h>

#include <SimpleMath.h>


class SquareRenderItem : public GameComponent
{
public:
  SquareRenderItem(RenderContext* context, DirectX::SimpleMath::Vector3 pos = { 0.0f, 0.0f, 0.0f }, DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f });

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool draw();

  ~SquareRenderItem();

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
};

