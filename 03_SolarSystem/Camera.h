#pragma once

#include <GameComponent.h>
#include <RenderContext.h>
#include <Window.h>

#include <SimpleMath.h>


class Camera : public GameComponent
{
public:
  Camera(RenderContext* context, Window* window);

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool updateSubresources();
  virtual bool draw();

  void moveCamera(DirectX::SimpleMath::Vector3 dir);
  void rotateCamera(DirectX::SimpleMath::Vector2 dir);

  void setPerspectiveProjection() { mPerspective = true; };
  void setOrthographicProjection() { mPerspective = false; };

  virtual ~Camera() = default;

protected:
  bool mPerspective = true;

  const float mMoveSpeed = 5.0f;
  const float mRotateSpeed = 50.0f;

  DirectX::SimpleMath::Vector3 mMoveDir = { 0.0, 0.0, 0.0 };
  DirectX::SimpleMath::Vector2 mRotateDir = { 0.0, 0.0 };

  RenderContext* mContext = nullptr;
  Window* mWindow = nullptr;

  DirectX::SimpleMath::Vector3 mPosition = { 10.0, 0.0, 0.0 };
  const DirectX::SimpleMath::Vector3 mDirectionDefault = { -1.0f, 0.0, 0.0 };
  const DirectX::SimpleMath::Vector3 mUpDefault = {0.0, 0.0, 1.0};

  float mUpRot = 0.0;
  float mRightRot = 0.0;

  DirectX::SimpleMath::Matrix mView = {};
  DirectX::SimpleMath::Matrix mProj = {};

  Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;
};

