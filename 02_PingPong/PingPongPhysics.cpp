#include "PingPongPhysics.h"

#include <algorithm>


PingPongPhysics::PingPongPhysics(DirectX::SimpleMath::Vector2 racketSize, float ballSize, float ballSpeedScale)
  : mBallSpeedScale(ballSpeedScale)
{
  mPlayer1.mPosition = { -1.0f + racketSize.x / 2.0f, 0.0f };
  mPlayer1.mSize = racketSize;  
  mPlayer1.mSpeed = 0.0f;
  
  mPlayer2.mPosition = { +1.0f - racketSize.x / 2.0f, 0.0f };
  mPlayer2.mSize = racketSize;
  mPlayer2.mSpeed = 0.0f;

  mBall.mPosition = { 0.0f, 0.0f };
  mBall.mSize = ballSize;
  mBall.mSpeed = { 0.0f, 0.0f };
}

bool PingPongPhysics::init()
{
  return true;
}

bool PingPongPhysics::update(float deltaTime)
{
  mPlayer1.mPosition.y = std::clamp(mPlayer1.mPosition.y + mPlayer1.mSpeed * deltaTime, -1.0f + mPlayer1.mSize.y, +1.0f - mPlayer1.mSize.y);
  mPlayer2.mPosition.y = std::clamp(mPlayer2.mPosition.y + mPlayer2.mSpeed * deltaTime, -1.0f + mPlayer2.mSize.y, +1.0f - mPlayer2.mSize.y);

  mPlayer1.mSpeed = 0.0f;
  mPlayer2.mSpeed = 0.0f;

  return true;
}

bool PingPongPhysics::draw()
{
  return true;
}
