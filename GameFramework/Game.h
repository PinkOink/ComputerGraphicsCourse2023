#pragma once

#include <vector>

#include "Window.h"
#include "RenderContext.h"
#include "GameComponent.h"


class Game
{
public:
  virtual bool init();
  bool run();

protected:
  Game(const char* gameName = "BaseGame");
  virtual ~Game();

  // This method must be implemented by children of Game
  virtual bool createGameComponents() = 0;

private:
  Game(const Game& rhs) = delete;
  Game(Game&& rhs) = delete;
  Game& operator=(const Game& rhs) = delete;
  Game& operator=(Game&& rhs) = delete;

  bool update();
  bool draw();
  
protected:
  Window* mWindow = nullptr;
  RenderContext* mRenderContext = nullptr;

protected:
  const char* mGameName;

  std::vector<GameComponent*> mGameComponents;
};

