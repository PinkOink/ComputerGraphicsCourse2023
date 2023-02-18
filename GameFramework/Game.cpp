#include "pch.h"
#include "Game.h"

#include <sstream>


bool Game::init()
{
	bool res = true;

	mWindow = new Window();
	res = mWindow->init(this, mGameName, 800, 600);
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

	mInputDevice = new InputDevice(mWindow);
	if (mInputDevice == nullptr)
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

	for (auto component : mGameComponents)
	{
		res = component->init();

		assert(res);
		if (!res) {
			break;
		}
	}

	return res;
}

bool Game::run()
{
	bool res = true;

	mTimer.Reset();

	while (!mWindow->shouldQuit())
	{
		mWindow->processMessages();

		if (!mPaused)
		{
			mTimer.Tick();

			updateFrameStats();

			if (mInputDevice->IsKeyDown(Keys::Escape))
			{
				mWindow->requestQuit();
			}

			res = update();
			assert(res);
			if (!res)
				return false;

			res = draw();
			assert(res);
			if (!res)
				return false;
		}
		else
		{
			Sleep(100);
		}
	}

	return true;
}

void Game::pause()
{
	mPaused = true;
}

void Game::unpause()
{
	mPaused = false;
}

bool Game::onResize(unsigned int width, unsigned int height)
{
	return (mRenderContext != nullptr) ? mRenderContext->onResize(width, height) : true;
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
		res = component->update(mTimer.DeltaTime());
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

void Game::updateFrameStats()
{
	static int frameCounter = 0;
	static float timeElapsed = 0.0f;

	frameCounter++;

	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCounter; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::ostringstream windowName;
		windowName.precision(6);
		windowName << mGameName << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << mspf << " (ms)";
		mWindow->rename(windowName.str().c_str());

		// Reset for next average.
		frameCounter = 0;
		timeElapsed += 1.0f;
	}
}
