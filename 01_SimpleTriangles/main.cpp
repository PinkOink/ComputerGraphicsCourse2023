#include "Game.h"


class GameChild : public Game
{
public:
  GameChild()
    : Game("SimpleTriangles")
  {}

protected:
  bool update()
  {
    return true;
  }
  bool draw()
  {
    mRenderContext->beginFrame();
    mRenderContext->endFrame();
    
    return true;
  }
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
  PSTR cmdLine, int showCmd)
{
  GameChild game;
  if (!game.init())
    return -1;

  game.run();

  return 0;
}