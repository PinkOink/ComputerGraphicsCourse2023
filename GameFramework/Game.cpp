#include "pch.h"
#include "Game.h"


bool Game::init()
{
	bool res = true;

	mWindow = new Window();
	res = mWindow->init(mGameName, 800, 600);
	if (!res)
	{
		MessageBox(0, L"Window initialization failed", 0, 0);
		assert(false);
		return false;
	}

	mRenderContext = new RenderContext();
	res = mRenderContext->init(mWindow);
	if (!res)
	{
		MessageBox(0, L"RenderContext initialization failed", 0, 0);
		assert(false);
		return false;
	}

	res = createGameComponents();
	if (!res)
	{
		MessageBox(0, L"GameComponents creation failed", 0, 0);
		assert(false);
		return false;
	}

	return res;
}

bool Game::run()
{
	bool res = true;

	while (!mWindow->shouldQuit())
	{
		mWindow->processMessages();

		res = update();
		assert(res);
		if (!res)
			return false;

		res = draw();
		assert(res);
		if (!res)
			return false;
	}

	return true;
}

Game::Game(const char* gameName)
	: mGameName(gameName)
{}

Game::~Game()
{
	delete mWindow;
	delete mRenderContext;
	for (auto component : mGameComponents)
	{
		delete component;
	}
}

bool Game::update()
{
	bool res = true;

	for (auto component : mGameComponents)
	{
		res = component->update();
		if (!res)
			return false;
	}

	return res;
}

bool Game::draw()
{
	bool res = true;

	mRenderContext->beginFrame();
	for (auto component : mGameComponents)
	{
		res = component->draw();
		if (!res)
			return false;
	}
	mRenderContext->endFrame();

	return res;
}
