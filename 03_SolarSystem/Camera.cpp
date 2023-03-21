#include "Camera.h"

#include <algorithm>


struct CameraCB
{
  DirectX::SimpleMath::Matrix viewProj;
};


Camera::Camera(RenderContext* context, Window* window)
  : mContext(context), mWindow(window)
{
}

bool Camera::init()
{
  mView = DirectX::SimpleMath::Matrix::CreateLookAt(mPosition, mPosition + mDirectionDefault, mUpDefault);
  mProj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(0.25f * 3.14f, (float)mWindow->getWidth() / (float)mWindow->getHeight(), 1.0f, 1000.0f);

  {
    CameraCB cb = {};
    cb.viewProj = (mView * mProj).Transpose();

    mConstantBuffer = mContext->createConstantBuffer(&cb, sizeof(cb));
  }

  return true;
}

bool Camera::update(float deltaTime)
{
  mUpRot = std::clamp(mUpRot + mRotateDir.y * mRotateSpeed * deltaTime, -80.0f, +80.0f);
  mRightRot += mRotateDir.x * mRotateSpeed * deltaTime;
  mRotateDir = { 0.0, 0.0 };

  DirectX::SimpleMath::Vector3 curDir = DirectX::SimpleMath::Vector3::Transform(mDirectionDefault, DirectX::SimpleMath::Matrix::CreateRotationY(-mUpRot * 3.14f / 180.0f));
  curDir = DirectX::SimpleMath::Vector3::Transform(curDir, DirectX::SimpleMath::Matrix::CreateRotationZ(-mRightRot * 3.14f / 180.0f));
  curDir.Normalize();

  DirectX::SimpleMath::Vector3 curRight = curDir.Cross(mUpDefault);
  curRight.Normalize();

  mPosition += curDir * mMoveDir.x * mMoveSpeed * deltaTime;
  mPosition += curRight * mMoveDir.y * mMoveSpeed * deltaTime;
  mPosition.z += 1.0f * mMoveDir.z * mMoveSpeed * deltaTime;
  mMoveDir = { 0.0, 0.0, 0.0 };

  mView = DirectX::SimpleMath::Matrix::CreateLookAt(mPosition, mPosition + curDir, mUpDefault);

  if (mPerspective)
  {
    mProj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(0.25f * 3.14f, (float)mWindow->getWidth() / (float)mWindow->getHeight(), 1.0, 100.0);
  }
  else
  {
    mProj = DirectX::SimpleMath::Matrix::CreateOrthographic((float)mWindow->getWidth() / (float)mWindow->getHeight() * 10.0f, 10.0f, 1.0, 100.0);
  }

  return true;
}

bool Camera::updateSubresources()
{
  CameraCB cb = {};
  cb.viewProj = (mView * mProj).Transpose();

  mContext->updateConstantBuffer(mConstantBuffer.Get(), &cb, sizeof(CameraCB));

  return true;
}

bool Camera::draw()
{
  mContext->mContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

  return true;
}

void Camera::moveCamera(DirectX::SimpleMath::Vector3 dir)
{
  mMoveDir = dir;
}

void Camera::rotateCamera(DirectX::SimpleMath::Vector2 dir)
{
  mRotateDir = 0.25 * dir;
}
