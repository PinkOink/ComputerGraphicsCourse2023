#pragma once

#include <GameComponent.h>
#include <RenderItem.h>

#include "CameraOrbit.h"


class PlayerComponent : public GameComponent
{
public:
  PlayerComponent(RenderItem* renderItem, RenderContext* context, Window* window, float radiusDefault);

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool updateSubresources();
  virtual bool draw();

  void rotateCamera(DirectX::SimpleMath::Vector2 dir);
  void addRadius(int step);

  const DirectX::SimpleMath::Matrix& getWorldMatrix() const { return mWorldMatrix; };

  virtual ~PlayerComponent();

protected:
  RenderItem* mRenderItem;
  CameraOrbit* mCamera;

  DirectX::SimpleMath::Matrix mLocalMatrix;
  DirectX::SimpleMath::Matrix mWorldMatrix;
};

