#include "pch.h"
#include "SphereRenderItem.h"

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


namespace
{
	struct MeshData
	{
		std::vector<Vertex> Vertices;
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
		Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f);
		Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

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

				Vertex v;

				// spherical to cartesian
				v.Position.x = radius * sinf(phi) * cosf(theta);
				v.Position.y = radius * cosf(phi);
				v.Position.z = radius * sinf(phi) * sinf(theta);

				v.Normal = v.Position;
				v.Normal.Normalize();

				v.TexC.x = theta / 3.141592654f / 2.0f;
				v.TexC.y = phi / 3.141592654f;

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


struct SphereCB
{
	DirectX::SimpleMath::Matrix transform;
	DirectX::SimpleMath::Matrix transformInv;
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
		mVertexBuffer = mContext->createVertexBuffer(meshData.Vertices.data(), (unsigned int)meshData.Vertices.size() * sizeof(Vertex));
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

		SphereCB cb = {};
		cb.transform = DirectX::SimpleMath::Matrix::CreateScale(mScale).Transpose();
		cb.transformInv = cb.transform.Invert().Transpose();
		cb.color = mColor;

		mConstantBuffer = mContext->createConstantBuffer(&cb, sizeof(cb));
	}

	// Create Vertex Shader and Input Layout
	if (SUCCEEDED(res))
	{
		ID3DBlob* vertexBC = nullptr;
		D3D_SHADER_MACRO macros[] = { "SPHERE", "1", NULL, NULL };

		mVertexShader = mContext->createVertexShader(vertexShaderFilename, &vertexBC, "main", macros);

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
		D3D_SHADER_MACRO macros[] = { "SPHERE", "1", NULL, NULL };

		mPixelShader = mContext->createPixelShader(pixelShaderFilename, "main", macros);
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
	SphereCB cb = {};
	cb.transform = mWorldMat.Transpose();
	cb.transformInv = cb.transform.Invert().Transpose();
	cb.color = mColor;

	mContext->updateConstantBuffer(mConstantBuffer.Get(), &cb, sizeof(SphereCB));

	return true;
}

bool SphereRenderItem::draw()
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

	mContext->mContext->DrawIndexedInstanced(mIndexNum, 1, 0, 0, 0);

	return true;
}

SphereRenderItem::~SphereRenderItem()
{}

