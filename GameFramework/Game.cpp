#include "pch.h"
#include "Game.h"


namespace
{
  Game* gGame = nullptr;
}


bool Game::init()
{
	if (!mWindow.init(mInstanceHandle, mGameName))
	{
		MessageBox(0, L"Window initialization failed", 0, 0);
		assert(false);
		return false;
	}

	return true;
}

int Game::run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

Game* Game::getGame()
{
  assert(gGame);
  return gGame;
}

LRESULT Game::msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

Game::Game(HINSTANCE hInstance)
	: mInstanceHandle(hInstance), mGameName(L"Game"), mWindow()
{
	assert(gGame == nullptr);
	gGame = this;
}

Game::~Game()
{
}
