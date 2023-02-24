#pragma once

#include <Game.h>

#include "PingPongPhysics.h"

class PingPongGame : public Game
{
public:
  PingPongGame();

protected:
  virtual bool createGameComponents();

  virtual void processInputDevice();

protected:
  bool mMenu = true;
  bool mPlayer2AI = false;

  PingPongPhysics* mPhys = nullptr;

  DirectX::SimpleMath::Vector2 mRacketSize = { 0.01f, 0.1f };
  float mRacketSpeed = 1.0f; // mRacketSpeed units in 1 second

  float mRacketAIZone = 0.05f;

  float mBallSize = 0.01f;
  float mBallStartSpeed = 0.5f;
};

