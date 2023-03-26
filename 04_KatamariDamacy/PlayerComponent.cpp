#include "PlayerComponent.h"

#include "ObjectComponent.h"

#include <algorithm>


PlayerComponent::PlayerComponent(RenderItem* renderItem, RenderContext* context, Window* window, float radiusDefault, float borderX, float borderY, std::vector<ObjectComponent*>& objects)
  : mRenderItem(renderItem),
  mCamera(new CameraOrbit(context, window, this)),
  mObjects(objects),
  mCurRadius(radiusDefault),
  mCurZ(radiusDefault),
  mPhysZ(radiusDefault),
  mBorderX(borderX),
  mBorderY(borderY)
{
}

bool PlayerComponent::init()
{
  mLocalMatrix = DirectX::SimpleMath::Matrix::Identity;
  mLocalStepMatrix = DirectX::SimpleMath::Matrix::Identity;
  mWorldMatrix = DirectX::SimpleMath::Matrix::CreateTranslation({ 0.0, 0.0, mCurZ });

  mRenderItem->setWorldMatrix(mWorldMatrix);

  mCamera->init();

  return true;
}

bool PlayerComponent::update(float deltaTime)
{
  DirectX::SimpleMath::Vector2 mPrevPos = mCurPos;
  if (mPhysZ > mCurZ) {
    mCurZ += std::fmin(mCorrectionZSpeed * deltaTime, mPhysZ - mCurZ);
  }
  DirectX::SimpleMath::Vector2 moveWorldForwardDir = mCamera->getPlaneDir();
  DirectX::SimpleMath::Vector2 moveWorldRightDir = { moveWorldForwardDir.y, -moveWorldForwardDir.x };
  moveWorldForwardDir.Normalize();
  moveWorldRightDir.Normalize();

  DirectX::SimpleMath::Vector2 moveWorld = moveWorldForwardDir * mMoveDir.x + moveWorldRightDir * mMoveDir.y;
  moveWorld.Normalize();
  mMoveDir = { 0.0, 0.0 };

  mCurPos.x = std::clamp(mCurPos.x + moveWorld.x * mMoveSpeed * deltaTime, -mBorderX, +mBorderX);
  mCurPos.y = std::clamp(mCurPos.y + moveWorld.y * mMoveSpeed * deltaTime, -mBorderY, +mBorderY);

  DirectX::SimpleMath::Vector3 moveWorldRight = { (mCurPos - mPrevPos).y, -(mCurPos - mPrevPos).x, 0.0 };

  mWorldMatrix = DirectX::SimpleMath::Matrix::CreateTranslation({ mCurPos.x, mCurPos.y, mCurZ });
  if ((mCurPos - mPrevPos).Length() > 0)
  {
    mLocalStepMatrix = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(moveWorldRight, -(mCurPos - mPrevPos).Length() / mCurRadius);
    mLocalMatrix *= mLocalStepMatrix;
  }
  else
  {
    mLocalStepMatrix = DirectX::SimpleMath::Matrix::Identity;
  }

  for (ObjectComponent* obj : mObjects)
  {
    if (obj->isFreeObject())
    {
      float objRadius = obj->getRadius();
      if (objRadius < mCurRadius)
      {
        DirectX::SimpleMath::Vector3 objPos = obj->getWorldPos();
        DirectX::SimpleMath::Vector3 dist = getWorldPos() - objPos;

        if (dist.Length() <= objRadius + mCurRadius)
        {
          obj->setPlayerComponent(this);
        }
      }
    }
  }

  mCamera->update(deltaTime);

  mRenderItem->setWorldMatrix(mLocalMatrix * mWorldMatrix);

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
