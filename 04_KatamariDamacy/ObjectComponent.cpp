#include "ObjectComponent.h"


ObjectComponent::ObjectComponent(RenderItem* renderItem, float radius, DirectX::SimpleMath::Vector2 pos)
  : mRenderItem(renderItem),
  mRadius(radius),
  mPos(pos)
{
}

bool ObjectComponent::init()
{
  mRenderItem->setWorldMatrix(DirectX::SimpleMath::Matrix::CreateTranslation({ mPos.x, mPos.y, mRadius }));

  return true;
}

bool ObjectComponent::update(float deltaTime)
{
  mRenderItem->setWorldMatrix(DirectX::SimpleMath::Matrix::CreateTranslation({ mPos.x, mPos.y, mRadius }));

  return true;
}

bool ObjectComponent::updateSubresources()
{
  mRenderItem->updateSubresources();

  return true;
}

bool ObjectComponent::draw()
{
  mRenderItem->draw();

  return true;
}

ObjectComponent::~ObjectComponent()
{
  delete mRenderItem;
}
