#include "pch.h"
#include "Game.h"


class GameChild : public Game
{
public:
  GameChild()
    : Game()
  {}
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