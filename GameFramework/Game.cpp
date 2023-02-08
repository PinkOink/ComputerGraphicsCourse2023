#include "pch.h"
#include "Game.h"


bool Game::init()
{
	mWindow = new Window();
	if (!mWindow->init(mGameName, 800, 600))
	{
		MessageBox(0, L"Window initialization failed", 0, 0);
		assert(false);
		return false;
	}

	mRenderContext = new RenderContext();
	if (!mRenderContext->init(mWindow))
	{
		MessageBox(0, L"RenderContext initialization failed", 0, 0);
		assert(false);
		return false;
	}

	return true;
}

bool Game::run()
{
	bool res = true;
	while (!mWindow->shouldQuit())
	{
		mWindow->processMessages();

		res = update();
		assert(res);

		res = draw();
		assert(res);
	}

	return true;
}

Game::Game()
	: mGameName("BaseGame")
{}

Game::~Game()
{
	delete mWindow;
	delete mRenderContext;
}
