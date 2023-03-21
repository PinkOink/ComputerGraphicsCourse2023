#pragma once

#include <Game.h>

#include "Camera.h"
#include "CameraOrbit.h"
#include "Planet.h"


class SolarSystemGame : public Game
{
public:
  SolarSystemGame();

  virtual ~SolarSystemGame();

protected:
  SolarSystemGame(const SolarSystemGame& rhs) = delete;
  SolarSystemGame(SolarSystemGame&& rhs) = delete;
  SolarSystemGame& operator=(const SolarSystemGame& rhs) = delete;
  SolarSystemGame& operator=(SolarSystemGame&& rhs) = delete;

  virtual bool createGameComponents() override;

  virtual void processInputDevice() override;
  virtual void processMouseMove(const InputDevice::MouseMoveEventArgs& args);

protected:
  Camera* mCamera = nullptr;
  CameraOrbit* mCameraOrbit = nullptr;

  Planet* mPlanets[9] = {};
};