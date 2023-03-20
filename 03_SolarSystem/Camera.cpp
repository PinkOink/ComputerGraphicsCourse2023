#include "Camera.h"


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
  mPosition = { 5.0, 5.0, -2.0 };
  mDirection = { -1.0f, -1.0, 0.0 };

  mView = DirectX::SimpleMath::Matrix::CreateLookAt(mPosition, mPosition + mDirection, { 0.0, 0.0, 1.0 });
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
  mView = DirectX::SimpleMath::Matrix::CreateLookAt(mPosition, mPosition + mDirection, { 0.0, 0.0, 1.0 });
  mProj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(0.25f * 3.14f, (float)mWindow->getWidth() / (float)mWindow->getHeight(), 1.0, 1000.0);

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
  DirectX::SimpleMath::Vector3 moveForward = mDirection;
  DirectX::SimpleMath::Vector3 moveRight = mDirection.Cross({ 0.0, 0.0, 1.0 });
  DirectX::SimpleMath::Vector3 moveUp = { 0.0, 0.0, 1.0 };

  DirectX::SimpleMath::Vector3 moveStep = moveForward * dir.x + moveRight * dir.y + moveUp * dir.z;

  moveStep.Normalize();

  mPosition += moveStep * mMoveSpeed;
}
