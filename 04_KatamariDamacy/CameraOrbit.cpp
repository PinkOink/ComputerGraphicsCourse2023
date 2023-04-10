#include "CameraOrbit.h"
#include "PlayerComponent.h"

#include <algorithm>


struct CameraOrbitCB
{
  DirectX::SimpleMath::Matrix viewProj;

  DirectX::SimpleMath::Vector3 cameraPosWorld;
  float padding;
};


CameraOrbit::CameraOrbit(RenderContext* context, Window* window, PlayerComponent* player)
  : mContext(context), mWindow(window), mPlayer(player)
{
}

bool CameraOrbit::init()
{
  mView = DirectX::SimpleMath::Matrix::CreateLookAt(mPositionDefault, mFocalDefault, mUpDefault);
  mProj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(0.25f * 3.14f, (float)mWindow->getWidth() / (float)mWindow->getHeight(), 1.0f, 1000.0f);

  {
    CameraOrbitCB cb = {};
    cb.viewProj = (mView * mProj).Transpose();
    cb.cameraPosWorld = mPositionDefault;

    mConstantBuffer = mContext->createConstantBuffer(&cb, sizeof(cb));
  }

  return true;
}

bool CameraOrbit::update(float deltaTime)
{
  mUpRot = std::clamp(mUpRot + mRotateDir.y * mRotateSpeed * deltaTime, 0.0f, +80.0f);
  mRightRot += mRotateDir.x * mRotateSpeed * deltaTime;
  mRotateDir = { 0.0, 0.0 };

  DirectX::SimpleMath::Vector3 curPos = DirectX::SimpleMath::Vector3::Transform(mPositionDefault, DirectX::SimpleMath::Matrix::CreateRotationY(-mUpRot * 3.14f / 180.0f));
  curPos = DirectX::SimpleMath::Vector3::Transform(curPos, DirectX::SimpleMath::Matrix::CreateRotationZ(-mRightRot * 3.14f / 180.0f));
  curPos.Normalize();

  mRadius = std::clamp(mRadius + mRadiusAdd * mRadiusSpeed * deltaTime, 0.01f, 1000.0f);
  mRadiusAdd = 0.0;
  curPos *= mRadius;

  mView = DirectX::SimpleMath::Matrix::CreateLookAt(curPos, mFocalDefault, mUpDefault);

  mProj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(0.25f * 3.14f, (float)mWindow->getWidth() / (float)mWindow->getHeight(), 1.0, 200.0);

  return true;
}

bool CameraOrbit::updateSubresources()
{
  DirectX::SimpleMath::Matrix planetMat = mPlayer->getWorldMatrix().Invert();

  CameraOrbitCB cb = {};
  cb.viewProj = (planetMat * mView * mProj).Transpose();
  cb.cameraPosWorld = DirectX::SimpleMath::Vector3::Transform(mPositionDefault, DirectX::SimpleMath::Matrix::CreateRotationY(-mUpRot * 3.14f / 180.0f));

  mContext->updateConstantBuffer(mConstantBuffer.Get(), &cb, sizeof(CameraOrbitCB));

  return true;
}

bool CameraOrbit::draw()
{
  mContext->mContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

  return true;
}

void CameraOrbit::rotateCamera(DirectX::SimpleMath::Vector2 dir)
{
  mRotateDir = 0.25 * dir;
}

void CameraOrbit::addRadius(int step)
{
  mRadiusAdd = (float)step;
}

DirectX::SimpleMath::Vector2 CameraOrbit::getPlaneDir()
{
  DirectX::SimpleMath::Vector3 dir = mPositionDefault;

  dir = DirectX::SimpleMath::Vector3::Transform(dir, DirectX::SimpleMath::Matrix::CreateRotationZ(-mRightRot * 3.14f / 180.0f));

  return DirectX::SimpleMath::Vector2(-dir.x, -dir.y);
}

