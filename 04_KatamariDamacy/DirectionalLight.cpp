#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(RenderContext* context, DirectionalLightCB lightData)
  : mContext(context), mLightData(lightData)
{
}

bool DirectionalLight::init()
{
  mConstantBuffer = mContext->createConstantBuffer(&mLightData, sizeof(mLightData));

  return true;
}

bool DirectionalLight::update(float deltaTime)
{
  return true;
}

bool DirectionalLight::updateSubresources()
{
  mContext->updateConstantBuffer(mConstantBuffer.Get(), &mLightData, sizeof(mLightData));

  return true;
}

bool DirectionalLight::draw()
{
  mContext->mContext->PSSetConstantBuffers(2, 1, mConstantBuffer.GetAddressOf());

  return true;
}

DirectionalLight::~DirectionalLight()
{}
