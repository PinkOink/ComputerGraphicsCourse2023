#pragma once

#include <GameComponent.h>
#include <RenderContext.h>
#include <Window.h>

#include <SimpleMath.h>

#include "Planet.h"


class CameraOrbit : public GameComponent
{
public:
  CameraOrbit(RenderContext* context, Window* window, Planet* planet);

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool updateSubresources();
  virtual bool draw();

  void activate() { mActivated = true; };
  void deactivate() { mActivated = false; };

  void useLocalTransform() { mUseLocalTransform = true; };
  void notUseLocalTransform() { mUseLocalTransform = false; };

  void rotateCamera(DirectX::SimpleMath::Vector2 dir);

  void setPlanet(Planet* planet) { 
    mUpRot = 0.0;
    mRightRot = 0.0;
    mPlanet = planet;
  };

  void setPerspectiveProjection() { mPerspective = true; };
  void setOrthographicProjection() { mPerspective = false; };

  virtual ~CameraOrbit() = default;

protected:
  bool mActivated = true;
  bool mUseLocalTransform = false;

  Planet* mPlanet;

  bool mPerspective = true;

  const float mRotateSpeed = 50.0f;

  DirectX::SimpleMath::Vector2 mRotateDir = { 0.0, 0.0 };

  RenderContext* mContext = nullptr;
  Window* mWindow = nullptr;

  DirectX::SimpleMath::Vector3 mPositionDefault = { 5.0, 0.0, 0.0 };
  const DirectX::SimpleMath::Vector3 mFocalDefault = { 0.0f, 0.0, 0.0 };
  const DirectX::SimpleMath::Vector3 mUpDefault = { 0.0, 0.0, 1.0 };

  float mUpRot = 0.0;
  float mRightRot = 0.0;

  DirectX::SimpleMath::Matrix mView = {};
  DirectX::SimpleMath::Matrix mProj = {};

  Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;
};

