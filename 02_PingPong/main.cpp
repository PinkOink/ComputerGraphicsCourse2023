#include "PingPongGame.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  PingPongGame game;
  if (!game.init())
    return -1;

  game.run();

  return 0;
}