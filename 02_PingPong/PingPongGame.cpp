#include "PingPongGame.h"

#include "PingPongPhysics.h"
#include "PingPongRender.h"


PingPongGame::PingPongGame()
  : Game("PingPong")
{}

bool PingPongGame::createGameComponents()
{
  mPhys = new PingPongPhysics(mRacketSize, mRacketSize.x, mBallSize, mBallStartSpeed, 1.05f);
  PingPongRender* renderer = new PingPongRender(mRenderContext, mPhys, mRacketSize, mBallSize);
  mPhys->pausePhysics();

  mGameComponents.push_back(mPhys);
  mGameComponents.push_back(renderer);

  return true;
}

void PingPongGame::processInputDevice()
{
  if (mMenu)
  {
    if (mInputDevice->IsKeyDown(Keys::D1))
    {
      mPlayer2AI = true;
      mMenu = false;

      mPhys->unpausePhysics();

      return;
    }
    if (mInputDevice->IsKeyDown(Keys::D2))
    {
      mPlayer2AI = false;
      mMenu = false;

      mPhys->unpausePhysics();

      return;
    }
  }
  else
  {
    // Check return to menu
    if (mInputDevice->IsKeyDown(Keys::Space))
    {
      mPhys->restartPhysics();
      mPhys->pausePhysics();

      mMenu = true;

      return;
    }

    // Player 1
    if (mInputDevice->IsKeyDown(Keys::W))
    {
      mPhys->addPlayer1Speed(+mRacketSpeed);
    }
    if (mInputDevice->IsKeyDown(Keys::S))
    {
      mPhys->addPlayer1Speed(-mRacketSpeed);
    }

    // Player 2
    if (mPlayer2AI)
    {
      // AI controlled
      float ballY = mPhys->getBallPos().y;
      float racketY = mPhys->getPlayer2Pos().y;

      if (ballY < racketY - mRacketAIZone)
      {
        mPhys->addPlayer2Speed(-mRacketSpeed);
      }
      if (ballY > racketY + mRacketAIZone)
      {
        mPhys->addPlayer2Speed(+mRacketSpeed);
      }
    }
    else
    {
      // Human controlled
      if (mInputDevice->IsKeyDown(Keys::Up))
      {
        mPhys->addPlayer2Speed(+mRacketSpeed);
      }
      if (mInputDevice->IsKeyDown(Keys::Down))
      {
        mPhys->addPlayer2Speed(-mRacketSpeed);
      }
    }
  }
}