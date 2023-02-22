#include "PingPongGame.h"

#include "SquareRenderItem.h"


PingPongGame::PingPongGame()
  : Game("PingPong")
{}

bool PingPongGame::createGameComponents()
{
  mKeysDelegate.AddLambda([](Keys key) {});

  mGameComponents.push_back(new SquareRenderItem(mRenderContext));

  return true;
}

void PingPongGame::processInputDevice()
{
  if (mInputDevice->IsKeyDown(Keys::W))
  {
    mKeysDelegate.Broadcast(Keys::W);
  }
}