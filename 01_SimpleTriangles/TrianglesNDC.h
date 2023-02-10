#pragma once
#include <GameComponent.h>
#include <RenderContext.h>


class TrianglesNDC : public GameComponent
{
public:
  TrianglesNDC(RenderContext* context);

  virtual bool init();
  virtual bool update();
  virtual bool draw();

  virtual ~TrianglesNDC();

protected:
  RenderContext* mContext;

  D3D11_PRIMITIVE_TOPOLOGY mTopology;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRastState;

  Microsoft::WRL::ComPtr<ID3D11InputLayout> mLayout = nullptr;

  Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader = nullptr;
  Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader = nullptr;

  // TODO: add strides and offsets to vertex buffer
  Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer = nullptr;
};

