#pragma once

#include <vector>

#include "Window.h"
#include "InputDevice.h"
#include "RenderContext.h"
#include "GameComponent.h"
#include "Timer.h"


class Game
{
public:
  virtual bool init();
  bool run();

  void pause();
  void unpause();

  bool onResize(unsigned int width, unsigned int height);

protected:
  Game(const char* gameName = "BaseGame");
  virtual ~Game();

  // This method must be implemented by children of Game
  virtual bool createGameComponents() = 0;

  virtual bool update();
  virtual bool draw();

  virtual void processInputDevice();

private:
  Game(const Game& rhs) = delete;
  Game(Game&& rhs) = delete;
  Game& operator=(const Game& rhs) = delete;
  Game& operator=(Game&& rhs) = delete;

  void updateFrameStats();
  
public:
  Window* mWindow = nullptr;
  RenderContext* mRenderContext = nullptr;
  InputDevice* mInputDevice = nullptr;

protected:
  const char* mGameName = nullptr;

  bool mPaused = false;

  Timer mTimer;

  std::vector<GameComponent*> mGameComponents;
};

