#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "Window.h"


// DirectX 11 Implementatnion. Maybe later hide behind an abstraction
class RenderContext final
{
public:
  RenderContext();
  ~RenderContext();

  bool init(Window* window);

  void beginFrame();
  void endFrame();

  RenderContext(const RenderContext& rhs) = delete;
  RenderContext(RenderContext&& rhs) = delete;
  RenderContext& operator=(const RenderContext& rhs) = delete;
  RenderContext& operator=(RenderContext&& rhs) = delete;

  // for easy acces to device and context
public:
  Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;

protected:
  Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mBackBufferView;
  Microsoft::WRL::ComPtr<ID3D11Texture2D> mBackBufferTex;

  // remove after render targets will be set manually
  float mWidth = 0;
  float mHeight = 0;
};

