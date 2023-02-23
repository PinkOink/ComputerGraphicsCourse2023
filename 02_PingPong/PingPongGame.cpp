#include "PingPongGame.h"

#include "PingPongPhysics.h"
#include "PingPongRender.h"


PingPongGame::PingPongGame()
  : Game("PingPong")
{}

bool PingPongGame::createGameComponents()
{
  DirectX::SimpleMath::Vector2 racketSize = { 0.02f, 0.1f };
  float ballSize = 0.01f;

  mPhys = new PingPongPhysics(racketSize, ballSize, 1.05f);
  PingPongRender* renderer = new PingPongRender(mRenderContext, mPhys, racketSize, ballSize);

  mGameComponents.push_back(mPhys);
  mGameComponents.push_back(renderer);

  return true;
}

void PingPongGame::processInputDevice()
{
  if (mInputDevice->IsKeyDown(Keys::W))
  {
    mPhys->addPlayer1Speed(+mRacketSpeed);
  }
  if (mInputDevice->IsKeyDown(Keys::S))
  {
    mPhys->addPlayer1Speed(-mRacketSpeed);
  }

  if (mInputDevice->IsKeyDown(Keys::Up))
  {
    mPhys->addPlayer2Speed(+mRacketSpeed);
  }
  if (mInputDevice->IsKeyDown(Keys::Down))
  {
    mPhys->addPlayer2Speed(-mRacketSpeed);
  }
}