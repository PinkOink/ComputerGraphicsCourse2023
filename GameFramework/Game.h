#pragma once

#include "Window.h"


class Game
{
public:
  bool init();
  bool run();

protected:
  Game();
  Game(const Game& rhs) = delete;
  Game(Game&& rhs) = delete;
  Game& operator=(const Game& rhs) = delete;
  Game& operator=(Game&& rhs) = delete;
  virtual ~Game();

public:
  Window* mWindow = nullptr;

protected:
  const char* mGameName;
};

