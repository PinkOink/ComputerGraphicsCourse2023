#include "DirectionalLight.h"


struct ShadowCB
{
	DirectX::SimpleMath::Matrix viewProj;
};


DirectionalLight::DirectionalLight(RenderContext* context, PlayerComponent* player, DirectionalLightCB lightData)
  : mContext(context), mPlayer(player), mLightData(lightData)
{
}

bool DirectionalLight::init()
{
  mConstantBuffer = mContext->createConstantBuffer(&mLightData, sizeof(mLightData));
	ShadowCB cb = {};
  mShadowCB = mContext->createConstantBuffer(&cb, sizeof(ShadowCB));

	// Create Shadow Map stuff
	{
		HRESULT res;

		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = mShadowMapSize;
		texDesc.Height = mShadowMapSize;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		res = mContext->mDevice->CreateTexture2D(&texDesc, nullptr, mShadowTex.GetAddressOf());
		assert(SUCCEEDED(res));

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		res = mContext->mDevice->CreateDepthStencilView(mShadowTex.Get(), &depthStencilViewDesc, mShadowTargetView.GetAddressOf());
		assert(SUCCEEDED(res));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		res = mContext->mDevice->CreateShaderResourceView(mShadowTex.Get(), &srvDesc, mShadowResourceView.GetAddressOf());
		assert(SUCCEEDED(res));

		mShadowViewport.TopLeftX = 0.0f;
		mShadowViewport.TopLeftY = 0.0f;
		mShadowViewport.Width = (FLOAT)mShadowMapSize;
		mShadowViewport.Height = (FLOAT)mShadowMapSize;
		mShadowViewport.MinDepth = 0.0f;
		mShadowViewport.MaxDepth = 1.0f;

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable = false;
		res = mContext->mDevice->CreateDepthStencilState(&depthStencilDesc, mShadowState.GetAddressOf());
		assert(SUCCEEDED(res));


		ID3DBlob* vertexBC = nullptr;
		mShadowVS = mContext->createVertexShader(L"./Resource/ShadowVS.hlsl", &vertexBC, "main", nullptr);

		D3D11_INPUT_ELEMENT_DESC inputElements[] = {
			D3D11_INPUT_ELEMENT_DESC {
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0},
		};

		res = mContext->mDevice->CreateInputLayout(
			inputElements,
			1,
			vertexBC->GetBufferPointer(),
			vertexBC->GetBufferSize(),
			&mShadowLayout);

		assert(SUCCEEDED(res));
	}

  return true;
}

bool DirectionalLight::update(float deltaTime)
{
  mLightData.lightDir = DirectX::SimpleMath::Vector3::TransformNormal(mLightData.lightDir, DirectX::SimpleMath::Matrix::CreateRotationX(mRotateSpeed * deltaTime));
  mLightData.lightDir.Normalize();

  return true;
}

bool DirectionalLight::updateSubresources()
{
  mContext->updateConstantBuffer(mConstantBuffer.Get(), &mLightData, sizeof(mLightData));

	std::vector<DirectX::SimpleMath::Vector4> corners = mPlayer->getCamera()->getFrustumCornersWorldSpace();
	DirectX::SimpleMath::Vector3 center = DirectX::SimpleMath::Vector3::Zero;
	for (auto& corner : corners)
	{
		center.x += corner.x;
		center.y += corner.y;
		center.z += corner.z;
	}
	center /= (float)corners.size();
	DirectX::SimpleMath::Vector3 up = { 0.0f, 0.0f, 1.0f };
	if (mLightData.lightDir.Cross(up).Length() < 1e-6)
	{
		up = { 0.0f, 1.0f, 0.0f };
	}
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::CreateLookAt(center, center - mLightData.lightDir, up);

	float xMin = -std::numeric_limits<float>::lowest();
	float yMin = -std::numeric_limits<float>::lowest();
	float zMin = -std::numeric_limits<float>::lowest();
	float xMax = std::numeric_limits<float>::lowest();
	float yMax = std::numeric_limits<float>::lowest();
	float zMax = std::numeric_limits<float>::lowest();
	for (auto& corner : corners)
	{
		DirectX::SimpleMath::Vector4 cornerPos = DirectX::SimpleMath::Vector4::Transform(corner, view);

		xMin = std::min<float>(xMin, cornerPos.x);
		yMin = std::min<float>(yMin, cornerPos.y);
		zMin = std::min<float>(zMin, cornerPos.z);
		xMax = std::max<float>(xMax, cornerPos.x);
		yMax = std::max<float>(yMax, cornerPos.y);
		zMax = std::max<float>(zMax, cornerPos.z);
	}
	DirectX::SimpleMath::Matrix proj = DirectX::SimpleMath::Matrix::CreateOrthographicOffCenter(xMin, xMax, yMin, yMax, zMin, zMax);

	ShadowCB cb = {};
	cb.viewProj = (view * proj).Transpose();
  mContext->updateConstantBuffer(mShadowCB.Get(), &cb, sizeof(ShadowCB));

  return true;
}

bool DirectionalLight::draw()
{
	mContext->mContext->ClearState();
	mContext->mContext->ClearDepthStencilView(mShadowTargetView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	mContext->mContext->IASetInputLayout(mShadowLayout.Get());
	mContext->mContext->VSSetShader(mShadowVS.Get(), nullptr, 0);
	mContext->mContext->VSSetConstantBuffers(0, 1, mShadowCB.GetAddressOf());
	mContext->mContext->RSSetViewports(1, &mShadowViewport);
	mContext->mContext->PSSetShader(nullptr, nullptr, 0);
	mContext->mContext->OMSetRenderTargets(0, nullptr, mShadowTargetView.Get());
	mContext->mContext->OMSetDepthStencilState(mShadowState.Get(), 0u);

	for (RenderItem* item : mGeometry)
	{
		item->drawGeometry();
	}

	mContext->beginFrame();
  mContext->mContext->PSSetConstantBuffers(2, 1, mConstantBuffer.GetAddressOf());

  return true;
}

DirectionalLight::~DirectionalLight()
{}
