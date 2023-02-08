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

	return true;
}

bool Game::run()
{
	while (!mWindow->shouldQuit())
	{
		mWindow->processMessages();
	}

	return true;
}

Game::Game()
	: mGameName("BaseGame")
{}

Game::~Game()
{
	delete mWindow;
}
