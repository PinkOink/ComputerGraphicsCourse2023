#include "pch.h"
#include "SphereRenderItem.h"

#include <SimpleMath.h>


namespace
{
	struct MeshData
	{
		std::vector<DirectX::SimpleMath::Vector4> Vertices;
		std::vector<int> Indices32;
	};

	MeshData CreateSphere(float radius, int sliceCount, int stackCount)
	{
		MeshData meshData;

		//
		// Compute the vertices stating at the top pole and moving down the stacks.
		//

		// Poles: note that there will be texture coordinate distortion as there is
		// not a unique point on the texture map to assign to the pole when mapping
		// a rectangular texture onto a sphere.
		DirectX::SimpleMath::Vector4 topVertex(0.0f, +radius, 0.0f, 1.0f);
		DirectX::SimpleMath::Vector4 bottomVertex(0.0f, -radius, 0.0f, 1.0f);

		meshData.Vertices.push_back(topVertex);

		float phiStep = 3.141592654f / stackCount;
		float thetaStep = -2.0f * 3.141592654f / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (int i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// Vertices of ring.
			for (int j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				DirectX::SimpleMath::Vector4 v;

				// spherical to cartesian
				v.x = radius * sinf(phi) * cosf(theta);
				v.y = radius * cosf(phi);
				v.z = radius * sinf(phi) * sinf(theta);
				v.w = 1.0f;

				meshData.Vertices.push_back(v);
			}
		}

		meshData.Vertices.push_back(bottomVertex);

		//
		// Compute indices for top stack.  The top stack was written first to the vertex buffer
		// and connects the top pole to the first ring.
		//

		for (int i = 1; i <= sliceCount; ++i)
		{
			meshData.Indices32.push_back(0);
			meshData.Indices32.push_back(i + 1);
			meshData.Indices32.push_back(i);
		}

		//
		// Compute indices for inner stacks (not connected to poles).
		//

		// Offset the indices to the index of the first vertex in the first ring.
		// This is just skipping the top pole vertex.
		int baseIndex = 1;
		int ringVertexCount = sliceCount + 1;
		for (int i = 0; i < stackCount - 2; ++i)
		{
			for (int j = 0; j < sliceCount; ++j)
			{
				meshData.Indices32.push_back(baseIndex + i * ringVertexCount + j);
				meshData.Indices32.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.Indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				meshData.Indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				meshData.Indices32.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.Indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		//
		// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
		// and connects the bottom pole to the bottom ring.
		//

		// South pole vertex was added last.
		int southPoleIndex = (int)meshData.Vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (int i = 0; i < sliceCount; ++i)
		{
			meshData.Indices32.push_back(southPoleIndex);
			meshData.Indices32.push_back(baseIndex + i);
			meshData.Indices32.push_back(baseIndex + i + 1);
		}

		return meshData;
	}
}


struct CircleCB
{
	DirectX::SimpleMath::Matrix transform;
	DirectX::SimpleMath::Vector4 color;
};


SphereRenderItem::SphereRenderItem(
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

	MeshData meshData = CreateSphere(1.0, 15, 15);

	// Create Vertex Buffer
	if (SUCCEEDED(res))
	{
		mVertexBuffer = mContext->createVertexBuffer(meshData.Vertices.data(), (unsigned int)meshData.Vertices.size() * sizeof(DirectX::SimpleMath::Vector4));
	}

	// Create Index Buffer
	if (SUCCEEDED(res))
	{
		mIndexNum = (int)meshData.Indices32.size();

		mIndexBuffer = mContext->createIndexBuffer(meshData.Indices32.data(), (unsigned int)meshData.Indices32.size() * sizeof(int));
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

		mVertexShader = mContext->createVertexShader(vertexShaderFilename, &vertexBC, "main");

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

void SphereRenderItem::setWorldMatrix(const DirectX::SimpleMath::Matrix& mat)
{
	mWorldMat = DirectX::SimpleMath::Matrix::CreateScale(mScale) * mat;
}

bool SphereRenderItem::updateSubresources()
{
	CircleCB cb = {};
	cb.transform = mWorldMat.Transpose();
	cb.color = mColor;

	mContext->updateConstantBuffer(mConstantBuffer.Get(), &cb, sizeof(CircleCB));

	return true;
}

bool SphereRenderItem::draw()
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

	mContext->mContext->DrawIndexedInstanced(mIndexNum, 1, 0, 0, 0);

	return true;
}

SphereRenderItem::~SphereRenderItem()
{}

