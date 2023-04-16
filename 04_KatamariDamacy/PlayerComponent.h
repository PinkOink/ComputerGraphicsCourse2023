#pragma once

#include <GameComponent.h>
#include <RenderItem.h>

#include "CameraOrbit.h"


class ObjectComponent;


class PlayerComponent : public GameComponent
{
public:
  PlayerComponent(RenderItem* renderItem, RenderContext* context, Window* window, float radiusDefault, float borderX, float borderY, std::vector<ObjectComponent*>& objects);

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool updateSubresources();
  virtual bool draw();

  void rotateCamera(DirectX::SimpleMath::Vector2 dir);
  void addRadius(int step);
  void addMove(DirectX::SimpleMath::Vector2& dir);

  const DirectX::SimpleMath::Matrix& getLocalMatrix() const { return mLocalMatrix; };
  const DirectX::SimpleMath::Matrix& getLocalAddMatrix() const { return mLocalStepMatrix; };
  const DirectX::SimpleMath::Matrix& getWorldMatrix() const { return mWorldMatrix; };
  DirectX::SimpleMath::Vector3 getWorldPos() const { return DirectX::SimpleMath::Vector3(mCurPos.x, mCurPos.y, mCurZ); }
  CameraOrbit* getCamera() { return mCamera; }

  virtual ~PlayerComponent();

protected:
  RenderItem* mRenderItem;
  CameraOrbit* mCamera;

  std::vector<ObjectComponent*>& mObjects;

  float mCurRadius = 0.0f;

  float mCurZ = 0.0f;
  float mPhysZ = 0.0f;

  const float mCorrectionZSpeed = 10.0f;
  const float mMoveSpeed = 10.0f;

  DirectX::SimpleMath::Vector2 mCurPos = { 0.0, 0.0 };
  DirectX::SimpleMath::Vector2 mMoveDir = { 0.0, 0.0 };

  DirectX::SimpleMath::Matrix mLocalMatrix;
  DirectX::SimpleMath::Matrix mLocalStepMatrix;
  DirectX::SimpleMath::Matrix mWorldMatrix;

  const float mBorderX;
  const float mBorderY;
};

