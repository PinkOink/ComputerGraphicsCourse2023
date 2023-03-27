#include "pch.h"
#include "MeshRenderItem.h"

#include <SimpleMath.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"



struct CircleCB
{
	DirectX::SimpleMath::Matrix transform;
	DirectX::SimpleMath::Vector4 color;
};


MeshRenderItem::MeshRenderItem(
	RenderContext* context,
	const std::string& meshFilename,
	const std::wstring& vertexShaderFilename,
	const std::wstring& pixelShaderFilename,
	DirectX::SimpleMath::Vector3 scale
)
	: mContext(context), mScale(scale)
{
	HRESULT res = S_OK;

	mTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	tinyobj::ObjReader reader;
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./Models/"; // Path to material files

	if (!reader.ParseFromFile(meshFilename, reader_config)) {
		assert(false);
	}

	if (!reader.Warning().empty()) {
		assert(false);
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	//auto& materials = reader.GetMaterials();

	std::vector<DirectX::SimpleMath::Vector3> positions;
	std::vector<DirectX::SimpleMath::Vector3> normals;
	std::vector<DirectX::SimpleMath::Vector2> texCoords;

	for (size_t shapeIdx = 0; shapeIdx < shapes.size(); shapeIdx++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;

		for (size_t faceIdx = 0; faceIdx < shapes[shapeIdx].mesh.num_face_vertices.size(); faceIdx++) {
			size_t faceVerticesNum = size_t(shapes[shapeIdx].mesh.num_face_vertices[faceIdx]);
			assert(faceVerticesNum == 3);

			// Loop over vertices in the face.
			for (size_t vertexIdx = 0; vertexIdx < faceVerticesNum; vertexIdx++) {
				// access to vertex
				tinyobj::index_t idx = shapes[shapeIdx].mesh.indices[index_offset + vertexIdx];

				positions.push_back({ 
					attrib.vertices[3 * size_t(idx.vertex_index) + 0], 
					attrib.vertices[3 * size_t(idx.vertex_index) + 1],
					attrib.vertices[3 * size_t(idx.vertex_index) + 2] 
					});

				// Check if `normal_index` is zero or positive. negative = no normal data
				assert(idx.normal_index >= 0);
				{
					normals.push_back({
						attrib.normals[3 * size_t(idx.normal_index) + 0],
						attrib.normals[3 * size_t(idx.normal_index) + 1],
						attrib.normals[3 * size_t(idx.normal_index) + 2]
						});
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				assert(idx.texcoord_index >= 0);
				{
					texCoords.push_back({
						attrib.texcoords[2 * size_t(idx.texcoord_index) + 0],
						attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]
						});
				}

				mVerticesNum++;
			}
			index_offset += faceVerticesNum;
		}
	}

	// Create Vertex Buffers
	if (SUCCEEDED(res))
	{
		mPositionsBuffer = mContext->createVertexBuffer(positions.data(), sizeof(DirectX::SimpleMath::Vector3) * mVerticesNum);
		mNormalsBuffer = mContext->createVertexBuffer(normals.data(), sizeof(DirectX::SimpleMath::Vector3) * mVerticesNum);
		mTexCoordsBuffer = mContext->createVertexBuffer(texCoords.data(), sizeof(DirectX::SimpleMath::Vector2) * mVerticesNum);
	}

	// Create Constant Buffer
	if (SUCCEEDED(res))
	{
		mWorldMat = DirectX::SimpleMath::Matrix::Identity;

		CircleCB cb = {};
		cb.transform = DirectX::SimpleMath::Matrix::CreateScale(mScale).Transpose();

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
				1,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0},
			D3D11_INPUT_ELEMENT_DESC {
				"TEXCOORD",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				2,
				0,
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

void MeshRenderItem::setWorldMatrix(const DirectX::SimpleMath::Matrix& mat)
{
	mWorldMat = DirectX::SimpleMath::Matrix::CreateScale(mScale) * mat;
}

bool MeshRenderItem::updateSubresources()
{
	CircleCB cb = {};
	cb.transform = mWorldMat.Transpose();

	mContext->updateConstantBuffer(mConstantBuffer.Get(), &cb, sizeof(CircleCB));

	return true;
}

bool MeshRenderItem::draw()
{
	mContext->mContext->RSSetState(mRastState.Get());

	mContext->mContext->IASetInputLayout(mLayout.Get());
	mContext->mContext->IASetPrimitiveTopology(mTopology);

	mContext->mContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mContext->mContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	ID3D11Buffer* vertexBuffers[] = { mPositionsBuffer.Get(), mNormalsBuffer.Get(), mTexCoordsBuffer.Get() };
	UINT strides[] = { sizeof(DirectX::SimpleMath::Vector3), sizeof(DirectX::SimpleMath::Vector3), sizeof(DirectX::SimpleMath::Vector2) };
	UINT offsets[] = { 0, 0, 0 };

	mContext->mContext->IASetVertexBuffers(0, 3, vertexBuffers, strides, offsets);

	mContext->mContext->VSSetConstantBuffers(1, 1, mConstantBuffer.GetAddressOf());

	mContext->mContext->Draw(mVerticesNum, 0);

	return true;
}

MeshRenderItem::~MeshRenderItem()
{}
