#pragma once

#include <GameComponent.h>
#include <RenderContext.h>
#include <Window.h>

#include <SimpleMath.h>

class PlayerComponent;


class CameraOrbit : public GameComponent
{
public:
  CameraOrbit(RenderContext* context, Window* window, PlayerComponent* player);

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool updateSubresources();
  virtual bool draw();

  void rotateCamera(DirectX::SimpleMath::Vector2 dir);
  void addRadius(int step);

  DirectX::SimpleMath::Vector2 getPlaneDir();
  std::vector<DirectX::SimpleMath::Vector4> getFrustumCornersWorldSpace();

  virtual ~CameraOrbit() = default;

protected:
  PlayerComponent* mPlayer;

  const float mRotateSpeed = 50.0f;
  const float mRadiusSpeed = 5.0f;

  DirectX::SimpleMath::Vector2 mRotateDir = { 0.0, 0.0 };
  float mRadiusAdd = 0.0f;

  RenderContext* mContext = nullptr;
  Window* mWindow = nullptr;

  DirectX::SimpleMath::Vector3 mPositionDefault = { 1.0, 0.0, 0.0 };
  const DirectX::SimpleMath::Vector3 mFocalDefault = { 0.0f, 0.0, 0.0 };
  const DirectX::SimpleMath::Vector3 mUpDefault = { 0.0, 0.0, 1.0 };

  float mUpRot = 0.0;
  float mRightRot = 0.0;
  float mRadius = 20.0f;

  DirectX::SimpleMath::Matrix mView = {};
  DirectX::SimpleMath::Matrix mProj = {};

  DirectX::SimpleMath::Vector3 mCurPos;

  Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;
};

