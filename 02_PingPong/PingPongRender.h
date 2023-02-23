#pragma once
#include <GameComponent.h>
#include <SimpleMath.h>

#include "PingPongPhysics.h"
#include "SquareRenderItem.h"
#include "CircleRenderItem.h"


class PingPongRender : public GameComponent
{
public:
  PingPongRender(RenderContext* context, const PingPongPhysics* physics, DirectX::SimpleMath::Vector2 racketSize, float ballSize);

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool draw();

  virtual ~PingPongRender() = default;

protected:
  const PingPongPhysics* mPhysics;

  RenderContext* mContext;

  SquareRenderItem mRacket1;
  SquareRenderItem mRacket2;

  CircleRenderItem mCircle;
};

