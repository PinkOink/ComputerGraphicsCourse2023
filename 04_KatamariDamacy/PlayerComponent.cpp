#include "PlayerComponent.h"

#include <algorithm>


PlayerComponent::PlayerComponent(RenderItem* renderItem, RenderContext* context, Window* window, float radiusDefault, float borderX, float borderY)
  : mRenderItem(renderItem), 
  mCamera(new CameraOrbit(context, window, this)), 
  mCurRadius(radiusDefault),
  mCurZ(radiusDefault),
  mPhysZ(radiusDefault),
  mBorderX(borderX),
  mBorderY(borderY)
{
}

bool PlayerComponent::init()
{
  mWorldMatrix = DirectX::SimpleMath::Matrix::CreateTranslation({ 0.0, 0.0, mCurZ });

  mRenderItem->setWorldMatrix(mWorldMatrix);

  mCamera->init();

  return true;
}

bool PlayerComponent::update(float deltaTime)
{
  if (mPhysZ > mCurZ) {
    mCurZ += std::fmin(mCorrectionZSpeed * deltaTime, mPhysZ - mCurZ);
  }
  DirectX::SimpleMath::Vector2 moveWorldForwardDir = mCamera->getPlaneDir();
  DirectX::SimpleMath::Vector2 moveWorldRightDir = { moveWorldForwardDir.y, -moveWorldForwardDir.x };
  moveWorldForwardDir.Normalize();
  moveWorldRightDir.Normalize();

  DirectX::SimpleMath::Vector2 moveWorldDir = moveWorldForwardDir * mMoveDir.x + moveWorldRightDir * mMoveDir.y;
  moveWorldDir.Normalize();
  mMoveDir = { 0.0, 0.0 };

  mCurPos.x = std::clamp(mCurPos.x + moveWorldDir.x * mMoveSpeed * deltaTime, -mBorderX, +mBorderX);
  mCurPos.y = std::clamp(mCurPos.y + moveWorldDir.y * mMoveSpeed * deltaTime, -mBorderY, +mBorderY);

  mWorldMatrix = DirectX::SimpleMath::Matrix::CreateTranslation({ mCurPos.x, mCurPos.y, mCurZ });

  mCamera->update(deltaTime);

  mRenderItem->setWorldMatrix(mWorldMatrix);

  return true;
}

bool PlayerComponent::updateSubresources()
{
  mCamera->updateSubresources();
  mRenderItem->updateSubresources();

  return true;
}

bool PlayerComponent::draw()
{
  mCamera->draw();
  mRenderItem->draw();

  return true;
}

void PlayerComponent::rotateCamera(DirectX::SimpleMath::Vector2 dir)
{
  mCamera->rotateCamera(dir);
}

void PlayerComponent::addRadius(int step)
{
  mCamera->addRadius(step);
}

void PlayerComponent::addMove(DirectX::SimpleMath::Vector2& dir)
{
  mMoveDir = dir;
}

PlayerComponent::~PlayerComponent()
{
  delete mCamera;
  delete mRenderItem;
}
