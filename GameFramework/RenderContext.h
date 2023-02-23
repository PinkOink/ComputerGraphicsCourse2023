#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <fstream>

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

  ID3D11Buffer* createVertexBuffer(void* vertexMem, unsigned int vertexSize);
  ID3D11Buffer* createIndexBuffer(void* indexMem, unsigned int indexSize);

  ID3D11Buffer* createConstantBuffer(void* initialMem, unsigned int bufferSize);
  void updateConstantBuffer(ID3D11Buffer* buf, void* updateMem, unsigned int bufferSize);

  ID3D11VertexShader* createVertexShader(const std::wstring& fileName, ID3DBlob** vsBlob, const std::string& entrypoint = "VSMain", const D3D_SHADER_MACRO* defines = nullptr);
  ID3D11PixelShader* createPixelShader(const std::wstring& fileName, const std::string& entrypoint = "PSMain", const D3D_SHADER_MACRO* defines = nullptr);

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

