#include "Planet.h"


Planet::Planet(
  const Planet* parent,
  RenderItem* renderItem,
  DirectX::SimpleMath::Vector3 initialPosition,
  float localRotationSpeed, 
  float parentRotationSpeed, 
  DirectX::SimpleMath::Vector3 localRotationAxis, 
  DirectX::SimpleMath::Vector3 parentRotationAxis
)
  : mParent(parent),
  mRenderItem(renderItem),
  mInitialPosition(initialPosition),
  mLocalRotationSpeed(localRotationSpeed),
  mParentRotationSpeed(parentRotationSpeed),
  mLocalRotationAxis(localRotationAxis),
  mParentRotationAxis(parentRotationAxis)
{
}

bool Planet::init()
{
  mWorldMatrix = DirectX::SimpleMath::Matrix::Identity;

  return true;
}

bool Planet::update(float deltaTime)
{
  mLocalRotationAngle += mLocalRotationSpeed * deltaTime;
  mParentRotationAngle += mParentRotationSpeed * deltaTime;

  mWorldMatrix = DirectX::SimpleMath::Matrix::Identity;

  // Local rotation
  mWorldMatrix = DirectX::SimpleMath::Matrix::Transform(mWorldMatrix, DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(mLocalRotationAxis, mLocalRotationAngle));

  // Move to parent's orbit in local space
  mWorldMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(mInitialPosition);

  // Rotate in parent's orbit
  mWorldMatrix = DirectX::SimpleMath::Matrix::Transform(mWorldMatrix, DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(mParentRotationAxis, mParentRotationAngle));

  // Move to world space
  if (mParent != nullptr)
  {
    mWorldMatrix *= mParent->getWorldMatrix();
  }

  mRenderItem->setWorldMatrix(mWorldMatrix);

  return true;
}

bool Planet::updateSubresources()
{
  return mRenderItem->updateSubresources();
}

bool Planet::draw()
{
  return mRenderItem->draw();
}

Planet::~Planet()
{
  delete mRenderItem;
}
