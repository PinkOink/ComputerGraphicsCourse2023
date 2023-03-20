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

  virtual ~Camera() = default;

protected:
  const float mMoveSpeed = 0.05f;

  RenderContext* mContext = nullptr;
  Window* mWindow = nullptr;

  DirectX::SimpleMath::Vector3 mPosition = {};
  DirectX::SimpleMath::Vector3 mDirection = {};

  DirectX::SimpleMath::Matrix mView = {};
  DirectX::SimpleMath::Matrix mProj = {};

  Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer = nullptr;
};

