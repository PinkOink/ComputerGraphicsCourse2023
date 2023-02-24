#pragma once
#include <GameComponent.h>
#include <Delegates.h>

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
  DirectX::SimpleMath::Vector2 mDir;
  float mSpeed;
  float mSize;
};


class PingPongPhysics : public GameComponent
{
public:
  PingPongPhysics(DirectX::SimpleMath::Vector2 racketSize, float racketOffset, float ballSize, float ballStartSpeed, float ballScaleSpeed);

  void addPlayer1Speed(float speed) { if (!mPaused) { mPlayer1.mSpeed += speed; } }
  void addPlayer2Speed(float speed) { if (!mPaused) { mPlayer2.mSpeed += speed; } }
  
  DirectX::SimpleMath::Vector2 getPlayer1Pos() const { return mPlayer1.mPosition; }
  DirectX::SimpleMath::Vector2 getPlayer2Pos() const { return mPlayer2.mPosition; }

  DirectX::SimpleMath::Vector2 getBallPos() const { return mBall.mPosition; }
  float getBallCurSpeed() const { return mBall.mSpeed; }
  float getBallStartSpeed() const { return mBallStartSpeed; }

  virtual bool init();
  virtual bool update(float deltaTime);
  virtual bool draw();

  void pausePhysics();
  void unpausePhysics();
  void restartPhysics();

  virtual ~PingPongPhysics() = default;

public:
  Delegate<void, int> mOnBallExit;

protected:
  bool mPaused = false;

  RacketPhysicBody mPlayer1;
  RacketPhysicBody mPlayer2;

  float mRacketOffset;

  BallPhysicBody mBall;

  float mBallStartSpeed;
  float mBallSpeedScale;
};

