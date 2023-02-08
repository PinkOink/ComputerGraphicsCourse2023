#include "GameSimpleTriangles.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  GameSimpleTriangles game;
  if (!game.init())
    return -1;

  game.run();

  return 0;
}