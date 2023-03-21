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

  virtual ~Camera() = default;

protected:
  const float mMoveSpeed = 5.0f;
  const float mRotateSpeed = 1.0f;

  DirectX::SimpleMath::Vector3 mMoveDir = { 0.0, 0.0, 0.0 };
  DirectX::SimpleMath::Vector2 mRotateDir = { 0.0, 0.0 };

  RenderContext* mContext = nullptr;
  Window* mWindow = nullptr;

  DirectX::SimpleMath::Vector3 mPosition = { 10.0, 0.0, 0.0 };
  DirectX::SimpleMath::Vector3 mDirection = { -1.0f, 0.0, 0.0 };
  DirectX::SimpleMath::Vector3 mUp = {0.0, 0.0, 1.0};

  DirectX::SimpleMath::Matrix mView = {};
  DirectX::SimpleMath::Matrix mViewInv = {};
  DirectX::SimpleMath::Matrix mProj = {};

  Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;
};

