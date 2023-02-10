#include "TrianglesNDC.h"

#include <iostream>

#include <DirectXMath.h>
#include <d3dcompiler.h>


TrianglesNDC::TrianglesNDC(RenderContext* context)
  : mContext(context)
{
}

bool TrianglesNDC::init()
{
	HRESULT res = S_OK;

  mTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// Create Vertex Buffer
	if (SUCCEEDED(res))
  {
		DirectX::XMFLOAT4 points[8] = {
			DirectX::XMFLOAT4( 0.5f,  0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4( 0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(-0.5f,  0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		};

		D3D11_BUFFER_DESC vertexBufDesc = {};
		vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufDesc.CPUAccessFlags = 0;
		vertexBufDesc.MiscFlags = 0;
		vertexBufDesc.StructureByteStride = 0;
		vertexBufDesc.ByteWidth = sizeof(points);

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = points;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		res = mContext->mDevice->CreateBuffer(&vertexBufDesc, &vertexData, &mVertexBuffer);

		assert(SUCCEEDED(res));
  }

	// Create Index Buffer
	if (SUCCEEDED(res))
	{
		int indeces[] = {
			0, 1, 2,
			1, 0, 3
		};
		D3D11_BUFFER_DESC indexBufDesc = {};
		indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufDesc.CPUAccessFlags = 0;
		indexBufDesc.MiscFlags = 0;
		indexBufDesc.StructureByteStride = 0;
		indexBufDesc.ByteWidth = sizeof(indeces);

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = indeces;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		res = mContext->mDevice->CreateBuffer(&indexBufDesc, &indexData, &mIndexBuffer);

		assert(SUCCEEDED(res));
	}

	// Create Vertex Shader and Input Layout
	if (SUCCEEDED(res))
	{
		ID3DBlob* vertexBC = nullptr;
		ID3DBlob* errorVertexCode = nullptr;

		res = D3DCompileFromFile(
			L"./Resource/SimpleTriangles.hlsl",
			nullptr /*macros*/,
			nullptr /*include*/,
			"VSMain",
			"vs_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&vertexBC,
			&errorVertexCode
		);

		if (FAILED(res)) {
			// If the shader failed to compile it should have written something to the error message.
			if (errorVertexCode) {
				char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

				std::cout << compileErrors << std::endl;
			}
			else
			{
				MessageBox(0, L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
			}

			return false;
		}

		res = mContext->mDevice->CreateVertexShader(
			vertexBC->GetBufferPointer(),
			vertexBC->GetBufferSize(),
			nullptr, &mVertexShader);

		assert(SUCCEEDED(res));

		D3D11_INPUT_ELEMENT_DESC inputElements[] = {
			D3D11_INPUT_ELEMENT_DESC {
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0},
			D3D11_INPUT_ELEMENT_DESC {
				"COLOR",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				sizeof(DirectX::XMFLOAT4),
				D3D11_INPUT_PER_VERTEX_DATA,
				0}
		};

		res = mContext->mDevice->CreateInputLayout(
			inputElements,
			sizeof(inputElements) / sizeof(D3D11_INPUT_ELEMENT_DESC),
			vertexBC->GetBufferPointer(),
			vertexBC->GetBufferSize(),
			&mLayout);

		assert(SUCCEEDED(res));
	}

	// Create pixel shader
	if (SUCCEEDED(res))
	{
		D3D_SHADER_MACRO shaderMacros[] = { 
			"TEST", "1", 
			"TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", 
			nullptr, nullptr 
		};

		ID3DBlob* pixelBC;
		ID3DBlob* errorPixelCode;
		res = D3DCompileFromFile(
			L"./Resource/SimpleTriangles.hlsl", 
			shaderMacros, 
			nullptr /*include*/, 
			"PSMain", 
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
				MessageBox(0, L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
			}

			return false;
		}

		res = mContext->mDevice->CreatePixelShader(
			pixelBC->GetBufferPointer(),
			pixelBC->GetBufferSize(),
			nullptr, &mPixelShader);		

		assert(SUCCEEDED(res));
	}

	// Create state
	if (SUCCEEDED(res))
	{
		CD3D11_RASTERIZER_DESC rastDesc = {};
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.FillMode = D3D11_FILL_SOLID;

		res = mContext->mDevice->CreateRasterizerState(&rastDesc, &mRastState);

		assert(SUCCEEDED(res));
	}

  return SUCCEEDED(res);
}

bool TrianglesNDC::update(float deltaTime)
{
  return true;
}

bool TrianglesNDC::draw()
{
	mContext->mContext->RSSetState(mRastState.Get());

	mContext->mContext->IASetInputLayout(mLayout.Get());
	mContext->mContext->IASetPrimitiveTopology(mTopology);

	mContext->mContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mContext->mContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	UINT strides[] = { sizeof(DirectX::XMFLOAT4) * 2 };
	UINT offsets[] = { 0 };
	mContext->mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), strides, offsets);
	mContext->mContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	mContext->mContext->DrawIndexedInstanced(6, 1, 0, 0, 0);

  return true;
}

TrianglesNDC::~TrianglesNDC()
{
}
