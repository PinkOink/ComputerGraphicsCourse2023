#include "SolarSystemGame.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  SolarSystemGame game;
  if (!game.init())
    return -1;

  game.run();

  return 0;
}