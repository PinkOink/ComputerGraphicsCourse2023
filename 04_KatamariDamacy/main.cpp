#include "KatamariDamacyGame.h"
#include <Game.h>


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  KatamariDamacyGame game;
  if (!game.init())
    return -1;

  game.run();

  return 0;
}