#pragma once

#include <Game.h>


class SolarSystemGame : public Game
{
public:
  SolarSystemGame();

  virtual ~SolarSystemGame();

protected:
  virtual bool createGameComponents();
};