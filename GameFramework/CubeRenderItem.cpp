#include "pch.h"
#include "CubeRenderItem.h"

#include <SimpleMath.h>


struct Vertex
{
	Vertex() {}
	Vertex(
		const DirectX::SimpleMath::Vector3& p,
		const DirectX::SimpleMath::Vector3& n,
		const DirectX::SimpleMath::Vector2& uv) :
		Position(p),
		Normal(n),
		TexC(uv) {}
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float u, float v) :
		Position(px, py, pz),
		Normal(nx, ny, nz),
		TexC(u, v) {}

	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector2 TexC;
};


struct CubeCB
{
	DirectX::SimpleMath::Matrix transform;
	DirectX::SimpleMath::Matrix transformInv;
	DirectX::SimpleMath::Vector4 color;
};


CubeRenderItem::CubeRenderItem(
	RenderContext* context,
	DirectX::SimpleMath::Vector3 scale,
	DirectX::SimpleMath::Vector4 color,
	const std::wstring& vertexShaderFilename,
	const std::wstring& pixelShaderFilename
)
	: mContext(context), mScale(scale), mColor(color)
{
	HRESULT res = S_OK;

	mTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// Create Vertex Buffer
	if (SUCCEEDED(res))
	{
		Vertex points[24];

		// Fill in the front face vertex data.
		points[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		points[1] = Vertex(-1.0f, +1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		points[2] = Vertex(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		points[3] = Vertex(+1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

		// Fill in the back face vertex data.
		points[4] = Vertex(-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		points[5] = Vertex(+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		points[6] = Vertex(+1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		points[7] = Vertex(-1.0f, +1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

		// Fill in the top face vertex data.
		points[8] = Vertex(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		points[9] = Vertex(-1.0f, +1.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		points[10] = Vertex(+1.0f, +1.0f, +1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		points[11] = Vertex(+1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

		// Fill in the bottom face vertex data.
		points[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
		points[13] = Vertex(+1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
		points[14] = Vertex(+1.0f, -1.0f, +1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
		points[15] = Vertex(-1.0f, -1.0f, +1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

		// Fill in the left face vertex data.
		points[16] = Vertex(-1.0f, -1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		points[17] = Vertex(-1.0f, +1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		points[18] = Vertex(-1.0f, +1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		points[19] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// Fill in the right face vertex data.
		points[20] = Vertex(+1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		points[21] = Vertex(+1.0f, +1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		points[22] = Vertex(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		points[23] = Vertex(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		mVertexBuffer = mContext->createVertexBuffer(points, sizeof(points));
	}

	// Create Index Buffer
	if (SUCCEEDED(res))
	{
		int indeces[36];

		// Fill in the front face index data
		indeces[0] = 1; indeces[1] = 0; indeces[2] = 2;
		indeces[3] = 2; indeces[4] = 0; indeces[5] = 3;

		// Fill in the back face index data
		indeces[6] = 5; indeces[7] = 4; indeces[8] = 6;
		indeces[9] = 6; indeces[10] = 4; indeces[11] = 7;

		// Fill in the top face index data
		indeces[12] = 9; indeces[13] = 8; indeces[14] = 10;
		indeces[15] = 10; indeces[16] = 8; indeces[17] = 11;

		// Fill in the bottom face index data
		indeces[18] = 13; indeces[19] = 12; indeces[20] = 14;
		indeces[21] = 14; indeces[22] = 12; indeces[23] = 15;

		// Fill in the left face index data
		indeces[24] = 17; indeces[25] = 16; indeces[26] = 18;
		indeces[27] = 18; indeces[28] = 16; indeces[29] = 19;

		// Fill in the right face index data
		indeces[30] = 21; indeces[31] = 20; indeces[32] = 22;
		indeces[33] = 22; indeces[34] = 20; indeces[35] = 23;

		mIndexBuffer = mContext->createIndexBuffer(indeces, sizeof(indeces));
	}

	// Create Constant Buffer
	if (SUCCEEDED(res))
	{
		mWorldMat = DirectX::SimpleMath::Matrix::Identity;

		CubeCB cb = {};
		cb.transform = DirectX::SimpleMath::Matrix::CreateScale(mScale).Transpose();
		cb.transformInv = cb.transform.Invert().Transpose();
		cb.color = mColor;

		mConstantBuffer = mContext->createConstantBuffer(&cb, sizeof(cb));
	}

	// Create Vertex Shader and Input Layout
	if (SUCCEEDED(res))
	{
		ID3DBlob* vertexBC = nullptr;

		mVertexShader = mContext->createVertexShader(vertexShaderFilename, &vertexBC, "main");

		D3D11_INPUT_ELEMENT_DESC inputElements[] = {
			D3D11_INPUT_ELEMENT_DESC {
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0},
			D3D11_INPUT_ELEMENT_DESC {
				"NORMAL",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				sizeof(DirectX::SimpleMath::Vector3),
				D3D11_INPUT_PER_VERTEX_DATA,
				0},
			D3D11_INPUT_ELEMENT_DESC {
				"TEXCOORD",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				0,
				2 * sizeof(DirectX::SimpleMath::Vector3),
				D3D11_INPUT_PER_VERTEX_DATA,
				0},
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
		mPixelShader = mContext->createPixelShader(pixelShaderFilename, "main");
	}

	// Create state
	if (SUCCEEDED(res))
	{
		D3D11_RASTERIZER_DESC rastDesc = {};
		rastDesc.CullMode = D3D11_CULL_BACK;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.DepthClipEnable = true;

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
	CubeCB cb = {};
	cb.transform = mWorldMat.Transpose();
	cb.transformInv = cb.transform.Invert().Transpose();
	cb.color = mColor;

	mContext->updateConstantBuffer(mConstantBuffer.Get(), &cb, sizeof(CubeCB));

	return true;
}

bool CubeRenderItem::draw()
{
	mContext->mContext->RSSetState(mRastState.Get());

	mContext->mContext->IASetInputLayout(mLayout.Get());
	mContext->mContext->IASetPrimitiveTopology(mTopology);

	mContext->mContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mContext->mContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	UINT strides[] = { sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector2) };
	UINT offsets[] = { 0 };
	mContext->mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), strides, offsets);
	mContext->mContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	mContext->mContext->VSSetConstantBuffers(1, 1, mConstantBuffer.GetAddressOf());

	mContext->mContext->DrawIndexedInstanced(36, 1, 0, 0, 0);

	return true;
}

CubeRenderItem::~CubeRenderItem()
{}
