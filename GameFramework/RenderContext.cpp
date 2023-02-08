#include "pch.h"
#include "RenderContext.h"

#include <DirectXColors.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")


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
	swapDesc.BufferCount = 2;
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
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
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

	res = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&mBackBufferTex));
	assert(SUCCEEDED(res));
	res = mDevice->CreateRenderTargetView(mBackBufferTex.Get(), nullptr, &mBackBufferView);
	assert(SUCCEEDED(res));

  return true;
}

void RenderContext::beginFrame()
{
	mContext->ClearState();

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(mWidth);
	viewport.Height = static_cast<float>(mHeight);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	mContext->RSSetViewports(1, &viewport);

	mContext->OMSetRenderTargets(1, mBackBufferView.GetAddressOf(), nullptr);
	mContext->ClearRenderTargetView(mBackBufferView.Get(), DirectX::Colors::Aqua);
}

void RenderContext::endFrame()
{
	mContext->OMSetRenderTargets(0, nullptr, nullptr);

	mSwapChain->Present(1, 0);
}
