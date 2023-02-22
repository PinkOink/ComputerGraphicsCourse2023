#pragma once

#include <Game.h>

class PingPongGame : public Game
{
public:
  PingPongGame();

protected:
  virtual bool createGameComponents();

  virtual void processInputDevice();

protected:
  MulticastDelegate<const Keys> mKeysDelegate;
};

