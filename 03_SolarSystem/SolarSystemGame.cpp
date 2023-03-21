#include "SolarSystemGame.h"


SolarSystemGame::SolarSystemGame()
  : Game("SolarSystem")
{}

SolarSystemGame::~SolarSystemGame()
{
}

bool SolarSystemGame::createGameComponents()
{
  mCamera = new Camera(mRenderContext, mWindow);

  mInputDevice->MouseMove.AddLambda([this](const InputDevice::MouseMoveEventArgs& args) { this->processMouseMove(args); });

  mPlanets[0] = new Planet(
    nullptr, 
    new CubeRenderItem(mRenderContext, { 1.0, 1.0, 1.0 }, { 1.0, 0.0, 1.0, 1.0 }), 
    { 0.0, 0.0, 0.0 }, 
    -1.0, 
    0.0, 
    { 0.0, 0.0, 1.0 }, 
    { 0.0, 0.0, 1.0 });

  mPlanets[1] = new Planet(
    mPlanets[0],
    new CubeRenderItem(mRenderContext, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 0.0, 1.0 }), 
    { 0.0, 5.0, 0.0 }, 
    2.0, 
    3.0, 
    { 0.0, 1.0, 0.0 }, 
    { 0.0, 0.0, 1.0 });

  mGameComponents.push_back(mCamera);
  mGameComponents.push_back(mPlanets[0]);
  mGameComponents.push_back(mPlanets[1]);

  return true;
}

void SolarSystemGame::processInputDevice()
{
  DirectX::SimpleMath::Vector3 moveDir = { 0.0, 0.0, 0.0 };

  if (mInputDevice->IsKeyDown(Keys::W)) {
    moveDir.x += 1.0f;
  }
  if (mInputDevice->IsKeyDown(Keys::S)) {
    moveDir.x -= 1.0f;
  }
  if (mInputDevice->IsKeyDown(Keys::D)) {
    moveDir.y += 1.0f;
  }
  if (mInputDevice->IsKeyDown(Keys::A)) {
    moveDir.y -= 1.0f;
  }
  if (mInputDevice->IsKeyDown(Keys::E)) {
    moveDir.z += 1.0f;
  }
  if (mInputDevice->IsKeyDown(Keys::Q)) {
    moveDir.z -= 1.0f;
  }

  mCamera->moveCamera(moveDir);
}

void SolarSystemGame::processMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
  mCamera->rotateCamera(args.Offset);
}
