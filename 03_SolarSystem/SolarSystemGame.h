#pragma once

#include <Game.h>

#include "Camera.h"
#include "CubeRenderItem.h"


class SolarSystemGame : public Game
{
public:
  SolarSystemGame();

  virtual ~SolarSystemGame();

protected:
  virtual bool createGameComponents() override;

  virtual void processInputDevice() override;

protected:
  Camera* mCamera = nullptr;
  CubeRenderItem* mCube = nullptr;
};