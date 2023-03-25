#include "KatamariDamacyGame.h"


KatamariDamacyGame::KatamariDamacyGame()
{
}

KatamariDamacyGame::~KatamariDamacyGame()
{
}

bool KatamariDamacyGame::createGameComponents()
{
  mInputDevice->MouseMove.AddLambda([this](const InputDevice::MouseMoveEventArgs& args) { this->processMouseMove(args); });

  auto floorRenderItem = new CubeRenderItem(mRenderContext, { 50.0, 50.0, 1.0 }, { 1.0, 1.0, 1.0, 1.0 });

  floorRenderItem->setWorldMatrix(DirectX::SimpleMath::Matrix::CreateTranslation({ 0.0, 0.0, -1.0 }));

  mFloor = new RenderItemWrapper(floorRenderItem);
  mPlayer = new PlayerComponent(new CubeRenderItem(mRenderContext, { 1.0, 1.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 }), mRenderContext, mWindow, 1.0);

  mGameComponents.push_back(mPlayer);
  mGameComponents.push_back(mFloor);

  return true;
}

void KatamariDamacyGame::processInputDevice()
{
  if (mInputDevice->IsKeyDown(Keys::Up)) {
    mPlayer->addRadius(-1);
  }
  if (mInputDevice->IsKeyDown(Keys::Down)) {
    mPlayer->addRadius(+1);
  }
}

void KatamariDamacyGame::processMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
  mPlayer->rotateCamera(args.Offset);
}