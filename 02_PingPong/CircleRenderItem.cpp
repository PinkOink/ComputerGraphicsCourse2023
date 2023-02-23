#include "CircleRenderItem.h"

struct CircleCB
{
	DirectX::SimpleMath::Matrix transform;
	DirectX::SimpleMath::Vector4 color;
};


CircleRenderItem::CircleRenderItem(
	RenderContext* context,
	unsigned int circleSectionsNum,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 scale,
	DirectX::SimpleMath::Vector4 color
)
	: mContext(context), mPos(pos), mScale(scale), mColor(color), mCircleSectionsNum(circleSectionsNum)
{
	HRESULT res = S_OK;

	assert(mCircleSectionsNum >= 3);

	mTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// Create Vertex Buffer
	if (SUCCEEDED(res))
	{
		std::vector<DirectX::SimpleMath::Vector4> points;

		points.reserve(mCircleSectionsNum + 1);

		points.push_back(DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.5f, 1.0f));

		DirectX::SimpleMath::Vector4 circlePoint = { 1.0f, 0.0f, 0.5f, 1.0f };
		DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationZ(2 * 3.14159265359f / mCircleSectionsNum);
		for (unsigned int i = 0; i < mCircleSectionsNum; ++i)
		{
			points.push_back(circlePoint);
			circlePoint = DirectX::SimpleMath::Vector4::Transform(circlePoint, rot);
		}

		mVertexBuffer = mContext->createVertexBuffer(points.data(), sizeof(DirectX::SimpleMath::Vector4) * (unsigned int)points.size());
	}

	// Create Index Buffer
	if (SUCCEEDED(res))
	{
		std::vector<int> indeces;

		indeces.reserve(mCircleSectionsNum * 3);

		for (unsigned int i = 0; i < mCircleSectionsNum - 1; ++i)
		{
			indeces.push_back(0);
			indeces.push_back(i + 1);
			indeces.push_back(i + 2);
		}

		indeces.push_back(0);
		indeces.push_back(mCircleSectionsNum);
		indeces.push_back(1);

		mIndexBuffer = mContext->createIndexBuffer(indeces.data(), sizeof(int) * (unsigned int)indeces.size());
	}

	// Create Constant Buffer
	if (SUCCEEDED(res))
	{
		CircleCB cb = {};
		cb.transform = (DirectX::SimpleMath::Matrix::CreateScale(mScale) * DirectX::SimpleMath::Matrix::CreateTranslation(mPos)).Transpose();
		cb.color = mColor;

		mConstantBuffer = mContext->createConstantBuffer(&cb, sizeof(cb));
	}

	// Create Vertex Shader and Input Layout
	if (SUCCEEDED(res))
	{
		ID3DBlob* vertexBC = nullptr;

		mVertexShader = mContext->createVertexShader(L"./Resource/Mesh2dVS.hlsl", &vertexBC, "main");

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
		mPixelShader = mContext->createPixelShader(L"./Resource/Mesh2dPS.hlsl", "main");
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
}

bool CircleRenderItem::draw()
{
	CircleCB cb = {};
	cb.transform = (DirectX::SimpleMath::Matrix::CreateScale(mScale) * DirectX::SimpleMath::Matrix::CreateTranslation(mPos)).Transpose();
	cb.color = mColor;

	mContext->updateConstantBuffer(mConstantBuffer.Get(), &cb, sizeof(CircleCB));


	mContext->mContext->RSSetState(mRastState.Get());

	mContext->mContext->IASetInputLayout(mLayout.Get());
	mContext->mContext->IASetPrimitiveTopology(mTopology);

	mContext->mContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mContext->mContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	UINT strides[] = { sizeof(DirectX::SimpleMath::Vector4) };
	UINT offsets[] = { 0 };
	mContext->mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), strides, offsets);
	mContext->mContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	mContext->mContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

	mContext->mContext->DrawIndexedInstanced(mCircleSectionsNum * 3, 1, 0, 0, 0);

	return true;
}

CircleRenderItem::~CircleRenderItem()
{}
