#pragma once

#include <Game.h>

#include "PingPongPhysics.h"

class PingPongGame : public Game
{
public:
  PingPongGame();

  virtual ~PingPongGame();

protected:
  virtual bool createGameComponents();

  virtual bool update();
  virtual void processInputDevice();

  void onBallExit(int side);

  void printMenuText();
  void printScore();

protected:
  enum class GAMESTATE
  {
    MENU,
    PLAY,
    SHOWWIN
  };

protected:
  GAMESTATE mState = GAMESTATE::MENU;
  bool mPlayer2AI = false;

  PingPongPhysics* mPhys = nullptr;

  DirectX::SimpleMath::Vector2 mRacketSize = { 0.02f, 0.1f };
  float mRacketSpeed = 1.0f; // mRacketSpeed units in 1 second

  float mRacketAIZone = 0.05f;

  float mBallSize = 0.03f;
  float mBallStartSpeed = 0.5f;

  float mShowWinTime = 2.0f; // seconds
  float mShowWinTimeCount = 0.0f; // seconds

  unsigned int mPlayer1Score = 0;
  unsigned int mPlayer2Score = 0;
};

