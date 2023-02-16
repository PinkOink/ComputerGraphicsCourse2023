#include "TrianglesNDC.h"

#include <iostream>

#include <DirectXMath.h>


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

		mVertexBuffer = mContext->createVertexBuffer(points, sizeof(points));
  }

	// Create Index Buffer
	if (SUCCEEDED(res))
	{
		int indeces[] = {
			0, 1, 2,
			1, 0, 3
		};

		mIndexBuffer = mContext->createIndexBuffer(indeces, sizeof(indeces));
	}

	// Create Vertex Shader and Input Layout
	if (SUCCEEDED(res))
	{
		ID3DBlob* vertexBC = nullptr;

		mVertexShader = mContext->createVertexShader(L"./Resource/SimpleTriangles.hlsl", &vertexBC);

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

		mPixelShader = mContext->createPixelShader(L"./Resource/SimpleTriangles.hlsl", "PSMain", shaderMacros);
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
