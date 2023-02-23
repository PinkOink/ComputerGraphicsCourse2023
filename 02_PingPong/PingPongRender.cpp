#include "PingPongRender.h"


PingPongRender::PingPongRender(RenderContext* context, const PingPongPhysics* physics, DirectX::SimpleMath::Vector2 racketSize, float ballSize)
  : mContext(context), 
  mPhysics(physics), 
  mRacket1(context, { physics->getPlayer1Pos().x, physics->getPlayer1Pos().y, 0.5f }, { racketSize.x, racketSize.y, 1.0f }),
  mRacket2(context, { physics->getPlayer2Pos().x, physics->getPlayer2Pos().y, 0.5f }, { racketSize.x, racketSize.y, 1.0f }),
  mCircle(context, 16, { physics->getBallPos().x, physics->getBallPos().y, 0.5f }, { ballSize, ballSize, 1.0f })
{}

bool PingPongRender::init()
{
  return true;
}

bool PingPongRender::update(float deltaTime)
{
  return true;
}

bool PingPongRender::draw()
{
  mRacket1.setPosition({ mPhysics->getPlayer1Pos().x, mPhysics->getPlayer1Pos().y, 0.5f });
  mRacket2.setPosition({ mPhysics->getPlayer2Pos().x, mPhysics->getPlayer2Pos().y, 0.5f });

  mCircle.setPosition({ mPhysics->getBallPos().x, mPhysics->getBallPos().y, 0.5f });

  mRacket1.draw();
  mRacket2.draw();

  mCircle.draw();

  return true;
}
