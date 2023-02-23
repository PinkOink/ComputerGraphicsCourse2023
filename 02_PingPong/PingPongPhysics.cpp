#include "PingPongPhysics.h"

#include <algorithm>
#include <random>


namespace
{
  DirectX::SimpleMath::Vector2 randomVectorRotation(DirectX::SimpleMath::Vector2 vec, float minRot, float maxRot)
  {
    static std::random_device rd{};
    static std::mt19937 gen{ rd() };
    static std::normal_distribution<> random(0.0, 0.1);

    float randRotAngle = (maxRot + minRot + (maxRot - minRot) * std::clamp((float)random(gen), -1.0f, +1.0f)) / 2.0f;
    DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::CreateRotationZ(randRotAngle);
    return DirectX::SimpleMath::Vector2::Transform(vec, rot);
  }
}


PingPongPhysics::PingPongPhysics(DirectX::SimpleMath::Vector2 racketSize, float ballSize, float ballStartSpeed, float ballScaleSpeed)
  : mBallSpeedScale(ballScaleSpeed)
{
  mPlayer1.mPosition = { -1.0f + racketSize.x / 2.0f, 0.0f };
  mPlayer1.mSize = racketSize;  
  mPlayer1.mSpeed = 0.0f;
  
  mPlayer2.mPosition = { +1.0f - racketSize.x / 2.0f, 0.0f };
  mPlayer2.mSize = racketSize;
  mPlayer2.mSpeed = 0.0f;

  mBall.mPosition = { 0.0f, 0.0f };
  mBall.mSize = ballSize;
  mBall.mSpeed = randomVectorRotation({ -1.0f, 0.0f }, -3.14f / 4.0f, +3.14f / 4.0f);
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


  mBall.mPosition.x = std::clamp(mBall.mPosition.x + mBall.mSpeed.x * deltaTime, -1.0f + mBall.mSize, +1.0f - mBall.mSize);
  mBall.mPosition.y = std::clamp(mBall.mPosition.y + mBall.mSpeed.y * deltaTime, -1.0f + mBall.mSize, +1.0f - mBall.mSize);

  return true;
}

bool PingPongPhysics::draw()
{
  return true;
}
