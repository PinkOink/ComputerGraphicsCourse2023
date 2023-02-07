#pragma once


#include "Window.h"


class Game
{
public:
  bool init();
  int run();

  static Game* getGame();
  virtual LRESULT msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
  Game(HINSTANCE hInstance);
  Game(const Game& rhs) = delete;
  Game& operator=(const Game& rhs) = delete;
  virtual ~Game();

protected:
  LPCWSTR mGameName;
  HINSTANCE mInstanceHandle;

  Window mWindow;
};

