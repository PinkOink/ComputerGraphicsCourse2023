#include "PlayerComponent.h"


PlayerComponent::PlayerComponent(RenderItem* renderItem, RenderContext* context, Window* window, float radiusDefault)
  : mRenderItem(renderItem), mCamera(new CameraOrbit(context, window, this))
{
}

bool PlayerComponent::init()
{
  mCamera->init();

  return true;
}

bool PlayerComponent::update(float deltaTime)
{
  mCamera->update(deltaTime);

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

PlayerComponent::~PlayerComponent()
{
  delete mCamera;
  delete mRenderItem;
}
