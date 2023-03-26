#pragma once

#include <GameComponent.h>
#include <RenderItem.h>

#include "CameraOrbit.h"


class PlayerComponent : public GameComponent
{
public:
  PlayerComponent(RenderItem* renderItem, RenderContext* context, Window* window, float radiusDefault, float borderX, float borderY);

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool updateSubresources();
  virtual bool draw();

  void rotateCamera(DirectX::SimpleMath::Vector2 dir);
  void addRadius(int step);
  void addMove(DirectX::SimpleMath::Vector2& dir);

  const DirectX::SimpleMath::Matrix& getLocalMatrix() const { return mLocalMatrix; };
  const DirectX::SimpleMath::Matrix& getWorldMatrix() const { return mWorldMatrix; };

  virtual ~PlayerComponent();

protected:
  RenderItem* mRenderItem;
  CameraOrbit* mCamera;

  float mCurRadius = 0.0f;

  float mCurZ = 0.0f;
  float mPhysZ = 0.0f;

  const float mCorrectionZSpeed = 50.0f;
  const float mMoveSpeed = 10.0f;

  DirectX::SimpleMath::Vector2 mCurPos = { 0.0, 0.0 };
  DirectX::SimpleMath::Vector2 mMoveDir = { 0.0, 0.0 };

  DirectX::SimpleMath::Matrix mLocalMatrix;
  DirectX::SimpleMath::Matrix mWorldMatrix;

  const float mBorderX;
  const float mBorderY;
};

