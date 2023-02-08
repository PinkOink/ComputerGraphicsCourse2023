#pragma once
#include <Game.h>

#include "TrianglesNDC.h"


class GameSimpleTriangles : public Game
{
public:
  GameSimpleTriangles()
    : Game("SimpleTriangles")
  {}

protected:
  virtual bool createGameComponents()
  {
    mGameComponents.push_back(new TrianglesNDC(mRenderContext));

    return true;
  }
};

