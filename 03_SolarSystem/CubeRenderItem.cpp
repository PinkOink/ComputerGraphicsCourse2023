#include "CubeRenderItem.h"

#include <SimpleMath.h>


struct CircleCB
{
	DirectX::SimpleMath::Matrix transform;
	DirectX::SimpleMath::Vector4 color;
};


CubeRenderItem::CubeRenderItem(
	RenderContext* context,
	DirectX::SimpleMath::Vector3 scale,
	DirectX::SimpleMath::Vector4 color
)
	: mContext(context), mScale(scale), mColor(color)
{
	HRESULT res = S_OK;

	mTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// Create Vertex Buffer
	if (SUCCEEDED(res))
	{
		DirectX::SimpleMath::Vector4 points[] = {
			DirectX::SimpleMath::Vector4(+1.0f, +1.0f, +1.0f, 1.0f),
			DirectX::SimpleMath::Vector4(-1.0f, -1.0f, +1.0f, 1.0f),
			DirectX::SimpleMath::Vector4(+1.0f, -1.0f, +1.0f, 1.0f),
			DirectX::SimpleMath::Vector4(-1.0f, +1.0f, +1.0f, 1.0f),
			
			DirectX::SimpleMath::Vector4(+1.0f, +1.0f, -1.0f, 1.0f),
			DirectX::SimpleMath::Vector4(-1.0f, -1.0f, -1.0f, 1.0f),
			DirectX::SimpleMath::Vector4(+1.0f, -1.0f, -1.0f, 1.0f),
			DirectX::SimpleMath::Vector4(-1.0f, +1.0f, -1.0f, 1.0f)
		};

		mVertexBuffer = mContext->createVertexBuffer(points, sizeof(points));
	}

	// Create Index Buffer
	if (SUCCEEDED(res))
	{
		int indeces[] = {
			0, 1, 3,
			0, 2, 1,

			3, 5, 7,
			3, 1, 5,

			0, 7, 4,
			0, 3, 7,

			4, 2, 0,
			4, 6, 2,

			1, 6, 5,
			1, 2, 6,

			7, 6, 4,
			7, 5, 6
		};

		mIndexBuffer = mContext->createIndexBuffer(indeces, sizeof(indeces));
	}

	// Create Constant Buffer
	if (SUCCEEDED(res))
	{
		mWorldMat = DirectX::SimpleMath::Matrix::Identity;

		CircleCB cb = {};
		cb.transform = DirectX::SimpleMath::Matrix::CreateScale(mScale).Transpose();
		cb.color = mColor;

		mConstantBuffer = mContext->createConstantBuffer(&cb, sizeof(cb));
	}

	// Create Vertex Shader and Input Layout
	if (SUCCEEDED(res))
	{
		ID3DBlob* vertexBC = nullptr;

		mVertexShader = mContext->createVertexShader(L"./Resource/Mesh3dVS.hlsl", &vertexBC, "main");

		D3D11_INPUT_ELEMENT_DESC inputElements[] = {
			D3D11_INPUT_ELEMENT_DESC {
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				0,
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

	// Create Pixel Shader
	if (SUCCEEDED(res))
	{
		mPixelShader = mContext->createPixelShader(L"./Resource/Mesh3dPS.hlsl", "main");
	}

	// Create state
	if (SUCCEEDED(res))
	{
		CD3D11_RASTERIZER_DESC rastDesc = {};
		rastDesc.CullMode = D3D11_CULL_BACK;
		rastDesc.FillMode = D3D11_FILL_SOLID;

		res = mContext->mDevice->CreateRasterizerState(&rastDesc, &mRastState);

		assert(SUCCEEDED(res));
	}
}

void CubeRenderItem::setWorldMatrix(const DirectX::SimpleMath::Matrix& mat)
{
	mWorldMat = DirectX::SimpleMath::Matrix::CreateScale(mScale) * mat;
}

bool CubeRenderItem::updateSubresources()
{
	CircleCB cb = {};
	cb.transform = mWorldMat.Transpose();
	cb.color = mColor;

	mContext->updateConstantBuffer(mConstantBuffer.Get(), &cb, sizeof(CircleCB));

	return true;
}

bool CubeRenderItem::draw()
{
	mContext->mContext->RSSetState(mRastState.Get());

	mContext->mContext->IASetInputLayout(mLayout.Get());
	mContext->mContext->IASetPrimitiveTopology(mTopology);

	mContext->mContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mContext->mContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	UINT strides[] = { sizeof(DirectX::SimpleMath::Vector4) };
	UINT offsets[] = { 0 };
	mContext->mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), strides, offsets);
	mContext->mContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	mContext->mContext->VSSetConstantBuffers(1, 1, mConstantBuffer.GetAddressOf());

	mContext->mContext->DrawIndexedInstanced(36, 1, 0, 0, 0);

	return true;
}

CubeRenderItem::~CubeRenderItem()
{}
