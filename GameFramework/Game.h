#pragma once

#include "Window.h"
#include "RenderContext.h"


class Game
{
public:
  virtual bool init();
  bool run();

protected:
  Game();
  Game(const Game& rhs) = delete;
  Game(Game&& rhs) = delete;
  Game& operator=(const Game& rhs) = delete;
  Game& operator=(Game&& rhs) = delete;
  virtual ~Game();


  // These methods must be implemented by child classes
  virtual bool update() = 0;
  virtual bool draw() = 0;

public:
  Window* mWindow = nullptr;
  RenderContext* mRenderContext = nullptr;

protected:
  const char* mGameName;
};

