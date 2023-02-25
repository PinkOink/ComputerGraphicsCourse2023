#include "PingPongGame.h"

#include "PingPongPhysics.h"
#include "PingPongRender.h"


PingPongGame::PingPongGame()
  : Game("PingPong")
{}

bool PingPongGame::createGameComponents()
{
  mPhys = new PingPongPhysics(mRacketSize, mRacketSize.x * 2, mBallSize, mBallStartSpeed, 1.05f);
  PingPongRender* renderer = new PingPongRender(mRenderContext, mPhys, mRacketSize, mBallSize);
  mPhys->pausePhysics();
  mPhys->mOnBallExit.BindLambda([this](int side) { this->onBallExit(side); });

  mGameComponents.push_back(mPhys);
  mGameComponents.push_back(renderer);

  return true;
}

bool PingPongGame::update()
{
  if (mState == GAMESTATE::SHOWWIN)
  {
    if (mShowWinTimeCount >= mShowWinTime)
    {
      mState = GAMESTATE::PLAY;
      mPhys->restartPhysics();
      mPhys->unpausePhysics();
    }
    else
    {
      mShowWinTimeCount += mTimer.DeltaTime();
    }
  }

  return Game::update();
}

void PingPongGame::processInputDevice()
{
  if (mState == GAMESTATE::MENU)
  {
    if (mInputDevice->IsKeyDown(Keys::D1))
    {
      mState = GAMESTATE::PLAY;
      mPlayer2AI = true;

      mPhys->unpausePhysics();

      return;
    }
    if (mInputDevice->IsKeyDown(Keys::D2))
    {
      mState = GAMESTATE::PLAY;
      mPlayer2AI = false;

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

      mState = GAMESTATE::MENU;

      return;
    }

    if (mState != GAMESTATE::SHOWWIN)
    {
      // Player 1
      if (mInputDevice->IsKeyDown(Keys::W) || (mPlayer2AI && mInputDevice->IsKeyDown(Keys::Up)))
      {
        mPhys->addPlayer1Speed(+mRacketSpeed);
      }
      if (mInputDevice->IsKeyDown(Keys::S) || (mPlayer2AI && mInputDevice->IsKeyDown(Keys::Down)))
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
      } else
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
}

void PingPongGame::onBallExit(int side)
{
  mPhys->pausePhysics();
  mState = GAMESTATE::SHOWWIN;
  mShowWinTimeCount = 0.0f;
}
