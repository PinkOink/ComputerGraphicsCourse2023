#pragma once
#include <GameComponent.h>

#include <SimpleMath.h>


struct RacketPhysicBody
{
  DirectX::SimpleMath::Vector2 mPosition;
  float mSpeed;
  DirectX::SimpleMath::Vector2 mSize;
};


struct BallPhysicBody
{
  DirectX::SimpleMath::Vector2 mPosition;
  DirectX::SimpleMath::Vector2 mSpeed;
  float mSize;
};


class PingPongPhysics : public GameComponent
{
public:
  PingPongPhysics(DirectX::SimpleMath::Vector2 racketSize, float ballSize, float ballSpeedScale);

  void addPlayer1Speed(float speed) { mPlayer1.mSpeed += speed; }
  void addPlayer2Speed(float speed) { mPlayer2.mSpeed += speed; }
  
  DirectX::SimpleMath::Vector2 getPlayer1Pos() const { return mPlayer1.mPosition; }
  DirectX::SimpleMath::Vector2 getPlayer2Pos() const { return mPlayer2.mPosition; }

  DirectX::SimpleMath::Vector2 getBallPos() const { return mBall.mPosition; }
  float getBallSpeedValue() const { return mBall.mSpeed.Length(); }

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool draw();

  virtual ~PingPongPhysics() = default;

protected:
  RacketPhysicBody mPlayer1;
  RacketPhysicBody mPlayer2;

  BallPhysicBody mBall;

  float mBallSpeedScale;
};

