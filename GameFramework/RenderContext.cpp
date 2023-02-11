#include "pch.h"
#include "RenderContext.h"

#include <DirectXColors.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")


static const unsigned int BACKBUFFER_COUNT = 2;


RenderContext::RenderContext()
{}

RenderContext::~RenderContext()
{}

bool RenderContext::init(Window* window)
{
	HRESULT res = S_OK;

	mWidth = (float)window->getWidth();
	mHeight = (float)window->getHeight();

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = BACKBUFFER_COUNT;
	swapDesc.BufferDesc.Width = window->getWidth();
	swapDesc.BufferDesc.Height = window->getHeight();
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = *(HWND*)window->getWindowHandle();
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = 0;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&mSwapChain,
		&mDevice,
		nullptr,
		&mContext);

	assert(SUCCEEDED(res));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferTex;
	res = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBufferTex));
	assert(SUCCEEDED(res));
	res = mDevice->CreateRenderTargetView(backBufferTex.Get(), nullptr, &mBackBufferView);
	assert(SUCCEEDED(res));

	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = mWidth;
	mViewport.Height = mHeight;
	mViewport.MinDepth = 0;
	mViewport.MaxDepth = 1.0f;

  return true;
}

bool RenderContext::onResize(unsigned int width, unsigned int height)
{
	mWidth = (float)width;
	mHeight = (float)height;

	HRESULT res = S_OK;

	mBackBufferView.Reset();

	res = mSwapChain->ResizeBuffers(BACKBUFFER_COUNT, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	assert(SUCCEEDED(res));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferTex;
	res = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBufferTex));
	assert(SUCCEEDED(res));
	res = mDevice->CreateRenderTargetView(backBufferTex.Get(), nullptr, &mBackBufferView);
	assert(SUCCEEDED(res));

	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = mWidth;
	mViewport.Height = mHeight;
	mViewport.MinDepth = 0;
	mViewport.MaxDepth = 1.0f;

	mContext->RSSetViewports(1, &mViewport);
	mContext->OMSetRenderTargets(1, mBackBufferView.GetAddressOf(), nullptr);

	return SUCCEEDED(res);
}

void RenderContext::beginFrame()
{
	mContext->ClearState();

	mContext->RSSetViewports(1, &mViewport);

	mContext->OMSetRenderTargets(1, mBackBufferView.GetAddressOf(), nullptr);
	mContext->ClearRenderTargetView(mBackBufferView.Get(), DirectX::Colors::Aqua);
}

void RenderContext::endFrame()
{
	mContext->OMSetRenderTargets(0, nullptr, nullptr);

	mSwapChain->Present(1, 0);
}
