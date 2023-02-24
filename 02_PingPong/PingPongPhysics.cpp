#include "PingPongPhysics.h"

#include <algorithm>
#include <random>

#include <DirectXCollision.h>


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

  bool checkMoveBallVsWall(
    DirectX::SimpleMath::Vector2 ballPos,
    DirectX::SimpleMath::Vector2 ballDir,
    float ballRadius,
    float ballMoveLength,
    DirectX::SimpleMath::Vector2 wallPos,
    DirectX::SimpleMath::Vector2 wallNormal,
    float& ballMovePossible
  )
  {
    // check if ray will cross plane from proper side
    float dn = ballDir.Dot(wallNormal);
    if (dn < -1e-6f /*0.0f*/)
    {
      // Add ball radius to wall pos
      DirectX::SimpleMath::Vector2 wallPosCorrected = wallPos + wallNormal * ballRadius;
      float dist = (wallPosCorrected - ballPos).Dot(wallNormal) / dn;

      if (0.0f <= dist && dist <= ballMoveLength)
      {
        ballMovePossible = dist;

        return true;
      }
    }
    return false;
  }

  bool checkMoveBallVsRacket(
    DirectX::SimpleMath::Vector2 ballPos,
    DirectX::SimpleMath::Vector2 ballDir,
    float ballRadius,
    float ballMoveLength,
    DirectX::SimpleMath::Vector2 rectCenter,
    DirectX::SimpleMath::Vector2 rectSize,
    float& ballMovePossible,
    DirectX::SimpleMath::Vector2& collisionNorm
  )
  {
    DirectX::BoundingBox box(DirectX::SimpleMath::Vector3(rectCenter.x, rectCenter.y, 0.0f), DirectX::SimpleMath::Vector3(rectSize.x + ballRadius, rectSize.y + ballRadius, 1.0f));

    float dist;

    if (box.Intersects(ballPos, ballDir, dist))
    {
      if (0.0f <= dist && dist <= ballMoveLength)
      {
        ballMovePossible = dist;

        DirectX::SimpleMath::Vector2 collisionPoint = ballPos + ballDir * dist;
        if (collisionPoint.x < rectCenter.x + rectSize.x + ballRadius)
        {
          if (collisionPoint.x > rectCenter.x - rectSize.x - ballRadius)
          {
            collisionNorm.x = 0.0f;
          }
          else
          {
            collisionNorm.x = -1.0f;
          }
        }
        else
        {
          collisionNorm.x = +1.0f;
        }
        if (collisionPoint.y < rectCenter.y + rectSize.y + ballRadius)
        {
          if (collisionPoint.y > rectCenter.y - rectSize.y - ballRadius)
          {
            collisionNorm.y = 0.0f;
          } else
          {
            collisionNorm.y = -1.0f;
          }
        } else
        {
          collisionNorm.y = +1.0f;
        }
        collisionNorm.Normalize();

        assert(collisionNorm.Length() > 0.01f);

        return true;
      }
    }
    return false;
  }
}


PingPongPhysics::PingPongPhysics(DirectX::SimpleMath::Vector2 racketSize, float racketOffset, float ballSize, float ballStartSpeed, float ballScaleSpeed)
  : mBallStartSpeed(ballStartSpeed), mBallSpeedScale(ballScaleSpeed), mRacketOffset(racketOffset)
{
  mPlayer1.mPosition = { -1.0f + racketOffset + racketSize.x / 2.0f, 0.0f };
  mPlayer1.mSize = racketSize;  
  mPlayer1.mSpeed = 0.0f;
  
  mPlayer2.mPosition = { +1.0f - racketOffset - racketSize.x / 2.0f, 0.0f };
  mPlayer2.mSize = racketSize;
  mPlayer2.mSpeed = 0.0f;

  mBall.mPosition = { 0.0f, 0.0f };
  mBall.mSize = ballSize;
  mBall.mDir = randomVectorRotation({ -1.0, 0.0f }, -3.14f / 4.0f, +3.14f / 4.0f);
  mBall.mSpeed = mBallStartSpeed;
}

bool PingPongPhysics::init()
{
  return true;
}

bool PingPongPhysics::update(float deltaTime)
{
  if (!mPaused)
  {
    mPlayer1.mPosition.y = std::clamp(mPlayer1.mPosition.y + mPlayer1.mSpeed * deltaTime, -1.0f + mPlayer1.mSize.y, +1.0f - mPlayer1.mSize.y);
    mPlayer2.mPosition.y = std::clamp(mPlayer2.mPosition.y + mPlayer2.mSpeed * deltaTime, -1.0f + mPlayer2.mSize.y, +1.0f - mPlayer2.mSize.y);

    mPlayer1.mSpeed = 0.0f;
    mPlayer2.mSpeed = 0.0f;

    // Try to move ball against 2 walls and 2 rackets
    float ballMoveReq = mBall.mSpeed * deltaTime;
    do
    {
      bool collisionDetected = false;
      bool racketCollisionDetected = false;

      DirectX::SimpleMath::Vector2 collisionNorm1;
      DirectX::SimpleMath::Vector2 collisionNorm2;

      float moveBeforePlayer1 = FLT_MAX;
      float moveBeforePlayer2 = FLT_MAX;
      float moveBeforeWall1 = FLT_MAX;
      float moveBeforeWall2 = FLT_MAX;

      racketCollisionDetected |= checkMoveBallVsRacket(mBall.mPosition, mBall.mDir, mBall.mSize, mBall.mSpeed * deltaTime, mPlayer1.mPosition, mPlayer1.mSize, moveBeforePlayer1, collisionNorm1);
      racketCollisionDetected |= checkMoveBallVsRacket(mBall.mPosition, mBall.mDir, mBall.mSize, mBall.mSpeed * deltaTime, mPlayer2.mPosition, mPlayer2.mSize, moveBeforePlayer2, collisionNorm2);

      collisionDetected |= racketCollisionDetected;

      collisionDetected |= checkMoveBallVsWall(mBall.mPosition, mBall.mDir, mBall.mSize, mBall.mSpeed * deltaTime, { 0.0f, +1.0f }, { 0.0f, -1.0f }, moveBeforeWall1);
      collisionDetected |= checkMoveBallVsWall(mBall.mPosition, mBall.mDir, mBall.mSize, mBall.mSpeed * deltaTime, { 0.0f, -1.0f }, { 0.0f, +1.0f }, moveBeforeWall2);

      float minBallMovePossible = min(min(min(moveBeforePlayer1, moveBeforePlayer2), min(moveBeforeWall1, moveBeforeWall2)), ballMoveReq);
      ballMoveReq -= minBallMovePossible;
      mBall.mPosition = mBall.mPosition + mBall.mDir * minBallMovePossible;

      // Reflect ball direction if collision happend
      if (collisionDetected && minBallMovePossible == moveBeforePlayer1)
      {
        mBall.mDir = DirectX::SimpleMath::Vector2::Reflect(mBall.mDir, collisionNorm1);
      }
      if (collisionDetected && minBallMovePossible == moveBeforePlayer2)
      {
        mBall.mDir = DirectX::SimpleMath::Vector2::Reflect(mBall.mDir, collisionNorm2);
      }
      if (collisionDetected && minBallMovePossible == moveBeforeWall1)
      {
        mBall.mDir = DirectX::SimpleMath::Vector2::Reflect(mBall.mDir, { 0.0f, -1.0f });
      }
      if (collisionDetected && minBallMovePossible == moveBeforeWall2)
      {
        mBall.mDir = DirectX::SimpleMath::Vector2::Reflect(mBall.mDir, { 0.0f, +1.0f });
      }

      if (racketCollisionDetected)
      {
        // Add speed after collision with racket
        mBall.mSpeed *= mBallSpeedScale;
      }
      if (collisionDetected)
      {
        // Add random rotation after collision
        mBall.mDir = randomVectorRotation(mBall.mDir, -3.14f / 4.0f, +3.14f / 4.0f);
      }

    } while (ballMoveReq > 1e-6);
  }

  return true;
}

bool PingPongPhysics::draw()
{
  return true;
}

void PingPongPhysics::pausePhysics()
{
  mPaused = true;
}

void PingPongPhysics::unpausePhysics()
{
  mPaused = false;
}

void PingPongPhysics::restartPhysics()
{
  mPlayer1.mPosition = { -1.0f + mRacketOffset + mPlayer1.mSize.x / 2.0f, 0.0f };
  mPlayer1.mSpeed = 0.0f;

  mPlayer2.mPosition = { +1.0f - mRacketOffset - mPlayer2.mSize.x / 2.0f, 0.0f };
  mPlayer2.mSpeed = 0.0f;

  mBall.mPosition = { 0.0f, 0.0f };
  mBall.mDir = randomVectorRotation({ -1.0, 0.0f }, -3.14f / 4.0f, +3.14f / 4.0f);
  mBall.mSpeed = mBallStartSpeed;

  mPaused = false;
}
