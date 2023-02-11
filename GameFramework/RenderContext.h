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

  bool onResize(unsigned int width, unsigned int height);

  void beginFrame();
  void endFrame();

  RenderContext(const RenderContext& rhs) = delete;
  RenderContext(RenderContext&& rhs) = delete;
  RenderContext& operator=(const RenderContext& rhs) = delete;
  RenderContext& operator=(RenderContext&& rhs) = delete;

  // for easy access to device and context
public:
  Microsoft::WRL::ComPtr<ID3D11Device> mDevice = nullptr;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext = nullptr;

protected:
  Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain = nullptr;

  D3D11_VIEWPORT mViewport = {};
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mBackBufferView = nullptr;

  // remove after render targets will be set manually
  float mWidth = 0;
  float mHeight = 0;
};

