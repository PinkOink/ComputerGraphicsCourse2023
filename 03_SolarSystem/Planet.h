#pragma once

#include <GameComponent.h>
#include <SimpleMath.h>

#include "RenderItem.h"


class Planet : public GameComponent
{
public:
  Planet(
    const Planet* parent,
    RenderItem* renderItem,
    DirectX::SimpleMath::Vector3 initialPosition,
    float localRotationSpeed, 
    float parentRotationSpeed, 
    DirectX::SimpleMath::Vector3 localRotationAxis,
    DirectX::SimpleMath::Vector3 parentRotationAxis
  );

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool updateSubresources();
  virtual bool draw();

  const DirectX::SimpleMath::Matrix& getWorldMatrix() const { return mWorldMatrix; };

  virtual ~Planet();

protected:
  const Planet* mParent;

  RenderItem* mRenderItem;

  const DirectX::SimpleMath::Vector3 mInitialPosition;

  const float mLocalRotationSpeed;
  const float mParentRotationSpeed;

  const DirectX::SimpleMath::Vector3 mLocalRotationAxis;
  const DirectX::SimpleMath::Vector3 mParentRotationAxis;

  float mLocalRotationAngle = 0.0f;
  float mParentRotationAngle = 0.0f;

  DirectX::SimpleMath::Matrix mLocalMatrix;
  DirectX::SimpleMath::Matrix mWorldMatrix;
};

