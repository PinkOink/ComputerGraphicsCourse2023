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
  PingPongPhysics* mPhys = nullptr;

  // mRacketSpeed units in 1 second
  float mRacketSpeed = 1.0f;
};

