#include "PingPongGame.h"

#include "SquareRenderItem.h"


PingPongGame::PingPongGame()
  : Game("PingPong")
{}

bool PingPongGame::createGameComponents()
{
  mKeysDelegate.AddLambda([](Keys key) {});

  mGameComponents.push_back(new SquareRenderItem(mRenderContext, DirectX::SimpleMath::Vector3(-0.85f, 0.0f, 0.0f), DirectX::SimpleMath::Vector3(0.01f, 0.1f, 1.0f)));
  mGameComponents.push_back(new SquareRenderItem(mRenderContext, DirectX::SimpleMath::Vector3(0.85f, 0.0f, 0.0f), DirectX::SimpleMath::Vector3(0.01f, 0.1f, 1.0f)));

  return true;
}

void PingPongGame::processInputDevice()
{
  if (mInputDevice->IsKeyDown(Keys::W))
  {
    mKeysDelegate.Broadcast(Keys::W);
  }
}