#include "pch.h"
#include "Game.h"


class GameChild : public Game
{
public:
  GameChild(HINSTANCE hInstance)
    : Game(hInstance)
  {}
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
  PSTR cmdLine, int showCmd)
{
  GameChild game(hInstance);
  if (!game.init())
    return 0;

  return game.run();
}