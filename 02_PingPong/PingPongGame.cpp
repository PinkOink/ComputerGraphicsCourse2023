#include "PingPongGame.h"

#include "PingPongPhysics.h"
#include "PingPongRender.h"

#include <iostream>

PingPongGame::PingPongGame()
  : Game("PingPong")
{
  AllocConsole();
  freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
  printMenuText();
}

PingPongGame::~PingPongGame()
{
  FreeConsole();
}

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

      std::cout << "PvE game started:" << std::endl;

      mPhys->unpausePhysics();

      return;
    }
    if (mInputDevice->IsKeyDown(Keys::D2))
    {
      mState = GAMESTATE::PLAY;
      mPlayer2AI = false;

      mPhys->unpausePhysics();

      std::cout << "PvP game started:" << std::endl;

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

      mPlayer1Score = 0;
      mPlayer2Score = 0;

      printMenuText();

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

  if (side < 0)
  {
    mPlayer1Score++;
  }
  if (side > 0)
  {
    mPlayer2Score++;
  }

  printScore();
}

void PingPongGame::printMenuText()
{
  std::cout << std::endl;
  std::cout << "Ping Pong Game" << std::endl;
  std::cout << std::endl;

  std::cout << "Press 1 to play PvE" << std::endl;
  std::cout << "Controlls:" << std::endl;
  std::cout << "  Use W/S or Up/Down arrows to move your racket" << std::endl;
  std::cout << std::endl;

  std::cout << "Press 2 to play PvP" << std::endl;
  std::cout << "Controlls:" << std::endl;
  std::cout << "  Use W/S arrows to move left racket" << std::endl;
  std::cout << "  Use Up/Down arrows to move right racket" << std::endl;
  std::cout << std::endl;
}

void PingPongGame::printScore()
{
  std::cout << mPlayer1Score << ':' << mPlayer2Score << std::endl;
}
