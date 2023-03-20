#pragma once

#include <GameComponent.h>
#include <RenderContext.h>


class TrianglesNDC : public GameComponent
{
public:
  TrianglesNDC(RenderContext* context);

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool updateSubresources();
  virtual bool draw();

  virtual ~TrianglesNDC();

protected:
  RenderContext* mContext;

  D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRastState = nullptr;

  Microsoft::WRL::ComPtr<ID3D11InputLayout> mLayout = nullptr;

  Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader = nullptr;
  Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader = nullptr;

  Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer = nullptr;
};

