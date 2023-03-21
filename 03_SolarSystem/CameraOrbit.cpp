#include "CameraOrbit.h"

#include <algorithm>


struct CameraOrbitCB
{
  DirectX::SimpleMath::Matrix viewProj;
};


CameraOrbit::CameraOrbit(RenderContext* context, Window* window, Planet* planet)
  : mContext(context), mWindow(window), mPlanet(planet)
{
}

bool CameraOrbit::init()
{
  mView = DirectX::SimpleMath::Matrix::CreateLookAt(mPositionDefault, mFocalDefault, mUpDefault);
  mProj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(0.25f * 3.14f, (float)mWindow->getWidth() / (float)mWindow->getHeight(), 1.0f, 1000.0f);

  {
    CameraOrbitCB cb = {};
    cb.viewProj = (mView * mProj).Transpose();

    mConstantBuffer = mContext->createConstantBuffer(&cb, sizeof(cb));
  }

  return true;
}

bool CameraOrbit::update(float deltaTime)
{
  if (mActivated)
  {
    mUpRot = std::clamp(mUpRot + mRotateDir.y * mRotateSpeed * deltaTime, -80.0f, +80.0f);
    mRightRot += mRotateDir.x * mRotateSpeed * deltaTime;
    mRotateDir = { 0.0, 0.0 };

    DirectX::SimpleMath::Vector3 curPos = DirectX::SimpleMath::Vector3::Transform(mPositionDefault, DirectX::SimpleMath::Matrix::CreateRotationY(-mUpRot * 3.14f / 180.0f));
    curPos = DirectX::SimpleMath::Vector3::Transform(curPos, DirectX::SimpleMath::Matrix::CreateRotationZ(-mRightRot * 3.14f / 180.0f));
    //curPos.Normalize();

    mView = DirectX::SimpleMath::Matrix::CreateLookAt(curPos, mFocalDefault, mUpDefault);

    if (mPerspective)
    {
      mProj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(0.25f * 3.14f, (float)mWindow->getWidth() / (float)mWindow->getHeight(), 1.0, 100.0);
    } else
    {
      mProj = DirectX::SimpleMath::Matrix::CreateOrthographic((float)mWindow->getWidth() / (float)mWindow->getHeight() * 10.0f, 10.0f, 1.0, 100.0);
    }
  }

  return true;
}

bool CameraOrbit::updateSubresources()
{
  if (mActivated)
  {
    DirectX::SimpleMath::Matrix planetMat = mPlanet->getWorldMatrix().Invert();

    CameraOrbitCB cb = {};
    cb.viewProj = (planetMat * mView * mProj).Transpose();

    mContext->updateConstantBuffer(mConstantBuffer.Get(), &cb, sizeof(CameraOrbitCB));
  }

  return true;
}

bool CameraOrbit::draw()
{
  if (mActivated)
  {
    mContext->mContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());
  }

  return true;
}

void CameraOrbit::rotateCamera(DirectX::SimpleMath::Vector2 dir)
{
  if (mActivated)
  {
    mRotateDir = 0.25 * dir;
  }
}

