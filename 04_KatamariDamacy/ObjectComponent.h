#pragma once

#include <GameComponent.h>
#include <RenderItem.h>

#include "PlayerComponent.h"


class ObjectComponent : public GameComponent
{
public:
  ObjectComponent(RenderItem* renderItem, float radius, DirectX::SimpleMath::Vector2 pos);

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool updateSubresources();
  virtual bool draw();

  void setPlayerComponent(PlayerComponent* player) { 
    mPlayer = player; 
    mLocalMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(getWorldPos() - mPlayer->getWorldPos());
  }
  bool isFreeObject() { return mPlayer == nullptr; }
  float getRadius() { return mRadius; }
  DirectX::SimpleMath::Vector3 getWorldPos() { return { mPos.x, mPos.y, mRadius }; }

  virtual ~ObjectComponent();

protected:
  RenderItem* mRenderItem;
  PlayerComponent* mPlayer = nullptr;

  const float mRadius;
  DirectX::SimpleMath::Vector2 mPos;

  DirectX::SimpleMath::Matrix mLocalMatrix;
};

