#include "pch.h"
#include "RenderContext.h"

#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <DDSTextureLoader.h>

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

	D3D11_TEXTURE2D_DESC depthStencilTexDesc = {};
	depthStencilTexDesc.ArraySize = 1;
	depthStencilTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTexDesc.CPUAccessFlags = 0;
	depthStencilTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilTexDesc.Width = window->getWidth();
	depthStencilTexDesc.Height = window->getHeight();
	depthStencilTexDesc.MipLevels = 1;
	depthStencilTexDesc.ArraySize = 1;
	depthStencilTexDesc.MiscFlags = 0;
	depthStencilTexDesc.SampleDesc.Count = 1;
	depthStencilTexDesc.SampleDesc.Quality = 0;
	depthStencilTexDesc.Usage = D3D11_USAGE_DEFAULT;

	res = mDevice->CreateTexture2D(&depthStencilTexDesc, NULL, mDepthStencilTex.GetAddressOf());
	assert(SUCCEEDED(res));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	res = mDevice->CreateDepthStencilView(mDepthStencilTex.Get(), &depthStencilViewDesc, mDepthStencilView.GetAddressOf());
	assert(SUCCEEDED(res));

	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = mWidth;
	mViewport.Height = mHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.StencilEnable = false;
	res = mDevice->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf());

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

	mDepthStencilTex.Reset();
	mDepthStencilView.Reset();

	D3D11_TEXTURE2D_DESC depthStencilTexDesc = {};
	depthStencilTexDesc.ArraySize = 1;
	depthStencilTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTexDesc.CPUAccessFlags = 0;
	depthStencilTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilTexDesc.Width = width;
	depthStencilTexDesc.Height = height;
	depthStencilTexDesc.MipLevels = 1;
	depthStencilTexDesc.ArraySize = 1;
	depthStencilTexDesc.MiscFlags = 0;
	depthStencilTexDesc.SampleDesc.Count = 1;
	depthStencilTexDesc.SampleDesc.Quality = 0;
	depthStencilTexDesc.Usage = D3D11_USAGE_DEFAULT;

	res = mDevice->CreateTexture2D(&depthStencilTexDesc, NULL, mDepthStencilTex.GetAddressOf());
	assert(SUCCEEDED(res));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	res = mDevice->CreateDepthStencilView(mDepthStencilTex.Get(), &depthStencilViewDesc, mDepthStencilView.GetAddressOf());
	assert(SUCCEEDED(res));

	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = mWidth;
	mViewport.Height = mHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.StencilEnable = false;
	res = mDevice->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf());

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

	mContext->ClearRenderTargetView(mBackBufferView.Get(), DirectX::Colors::Aqua);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	mContext->OMSetRenderTargets(1, mBackBufferView.GetAddressOf(), mDepthStencilView.Get());

	mContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0u);
}

void RenderContext::endFrame()
{
	mContext->OMSetRenderTargets(0, nullptr, nullptr);

	mSwapChain->Present(1, 0);
}

ID3D11Buffer* RenderContext::createVertexBuffer(void* vertexMem, unsigned int vertexSize)
{
	HRESULT res = S_OK;

	ID3D11Buffer* buf = nullptr;

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = vertexSize;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertexMem;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	res = mDevice->CreateBuffer(&vertexBufDesc, &vertexData, &buf);

	assert(SUCCEEDED(res));

	return buf;
}

ID3D11Buffer* RenderContext::createIndexBuffer(void* indexMem, unsigned int indexSize)
{
	HRESULT res = S_OK;

	ID3D11Buffer* buf = nullptr;

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = indexSize;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indexMem;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	res = mDevice->CreateBuffer(&indexBufDesc, &indexData, &buf);

	assert(SUCCEEDED(res));

	return buf;
}

ID3D11Buffer* RenderContext::createConstantBuffer(void* initialMem, unsigned int bufferSize)
{
	HRESULT res = S_OK;

	ID3D11Buffer* buf = nullptr;

	D3D11_BUFFER_DESC constantBufDesc = {};
	constantBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufDesc.MiscFlags = 0;
	constantBufDesc.StructureByteStride = 0;
	constantBufDesc.ByteWidth = bufferSize;

	D3D11_SUBRESOURCE_DATA cbData = {};
	cbData.pSysMem = initialMem;
	cbData.SysMemPitch = 0;
	cbData.SysMemSlicePitch = 0;

	res = mDevice->CreateBuffer(&constantBufDesc, &cbData, &buf);

	assert(SUCCEEDED(res));

	return buf;
}

void RenderContext::updateConstantBuffer(ID3D11Buffer* buf, void* updateMem, unsigned int bufferSize)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	mContext->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, updateMem, bufferSize);
	mContext->Unmap(buf, 0);
}

void RenderContext::createTexture(const std::wstring& texstureFileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView)
{
	DirectX::CreateDDSTextureFromFile(mDevice.Get(), texstureFileName.c_str(), texture, textureView);
}

ID3D11VertexShader* RenderContext::createVertexShader(const std::wstring& fileName, ID3DBlob** vsBlob, const std::string& entrypoint, const D3D_SHADER_MACRO* defines)
{
	HRESULT res = S_OK;

	ID3D11VertexShader* vs = nullptr;
	ID3DBlob* errorVertexCode = nullptr;

	res = D3DCompileFromFile(
		fileName.c_str(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(),
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		vsBlob,
		&errorVertexCode
	);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		} else
		{
			MessageBox(0, fileName.c_str(), L"Missing Shader File", MB_OK);
		}

		return nullptr;
	}

	res = mDevice->CreateVertexShader(
		(*vsBlob)->GetBufferPointer(),
		(*vsBlob)->GetBufferSize(),
		nullptr, &vs);

	assert(SUCCEEDED(res));

	return vs;
}

ID3D11PixelShader* RenderContext::createPixelShader(const std::wstring& fileName, const std::string& entrypoint, const D3D_SHADER_MACRO* defines)
{
	HRESULT res = S_OK;

	ID3D11PixelShader* ps = nullptr;
	ID3DBlob* pixelBC = nullptr;
	ID3DBlob* errorPixelCode = nullptr;

	res = D3DCompileFromFile(
		fileName.c_str(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(),
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelBC,
		&errorPixelCode
	);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorPixelCode) {
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		} else
		{
			MessageBox(0, fileName.c_str(), L"Missing Shader File", MB_OK);
		}

		return nullptr;
	}

	res = mDevice->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &ps);

	assert(SUCCEEDED(res));

	return ps;
}
