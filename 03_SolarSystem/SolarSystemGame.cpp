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

  {
    DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f };
    DirectX::SimpleMath::Vector4 color = { 1.0f, 0.0f, 1.0f, 1.0f };

    DirectX::SimpleMath::Vector3 initialPosition = { 0.0, 0.0, 0.0 };
    float localRotationSpeed = -1.0;
    float parentRotationSpeed = 0.0;
    DirectX::SimpleMath::Vector3 localRotationAxis = { 0.0, 0.0, 1.0 };
    DirectX::SimpleMath::Vector3 parentRotationAxis = { 0.0, 0.0, 1.0 };

    mPlanets[0] = new Planet(
      nullptr,
      new CubeRenderItem(mRenderContext, scale, color),
      initialPosition,
      localRotationSpeed,
      parentRotationSpeed,
      localRotationAxis,
      parentRotationAxis);
  }

  {    
    DirectX::SimpleMath::Vector3 scale = { 0.7f, 0.7f, 0.7f };
    DirectX::SimpleMath::Vector4 color = { 1.0f, 1.0f, 0.0f, 1.0f };

    DirectX::SimpleMath::Vector3 initialPosition = { 0.0, 5.0, 0.0 };
    float localRotationSpeed = 0.0;
    float parentRotationSpeed = 3.0;
    DirectX::SimpleMath::Vector3 localRotationAxis = { 0.0, 0.0, 1.0 };
    DirectX::SimpleMath::Vector3 parentRotationAxis = { 0.0, 0.0, 1.0 };

    mPlanets[1] = new Planet(
      mPlanets[0],
      new CubeRenderItem(mRenderContext, scale, color),
      initialPosition,
      localRotationSpeed,
      parentRotationSpeed,
      localRotationAxis,
      parentRotationAxis);
  }

  {    
    DirectX::SimpleMath::Vector3 scale = { 0.3f, 0.3f, 0.3f };
    DirectX::SimpleMath::Vector4 color = { 1.0f, 0.0f, 0.0f, 1.0f };

    DirectX::SimpleMath::Vector3 initialPosition = { 0.0, 0.0, 2.0 };
    float localRotationSpeed = 0.0;
    float parentRotationSpeed = 3.0;
    DirectX::SimpleMath::Vector3 localRotationAxis = { 0.0, 0.0, 1.0 };
    DirectX::SimpleMath::Vector3 parentRotationAxis = { 0.0, 1.0, 0.0 };

    mPlanets[2] = new Planet(
      mPlanets[1],
      new CubeRenderItem(mRenderContext, scale, color),
      initialPosition,
      localRotationSpeed,
      parentRotationSpeed,
      localRotationAxis,
      parentRotationAxis);
  }

  {    
    DirectX::SimpleMath::Vector3 scale = { 0.4f, 0.2f, 0.1f };
    DirectX::SimpleMath::Vector4 color = { 0.5f, 0.3f, 0.1f, 1.0f };

    DirectX::SimpleMath::Vector3 initialPosition = { 8.0, 0.0, 0.0 };
    float localRotationSpeed = 6.0;
    float parentRotationSpeed = 4.0;
    DirectX::SimpleMath::Vector3 localRotationAxis = { 0.0, 1.0, 1.0 };
    DirectX::SimpleMath::Vector3 parentRotationAxis = { 0.0, -1.0, 1.0 };

    mPlanets[3] = new Planet(
      mPlanets[0],
      new CubeRenderItem(mRenderContext, scale, color),
      initialPosition,
      localRotationSpeed,
      parentRotationSpeed,
      localRotationAxis,
      parentRotationAxis);
  }

  {    
    DirectX::SimpleMath::Vector3 scale = { 0.1f, 0.1f, 0.1f };
    DirectX::SimpleMath::Vector4 color = { 1.0f, 0.0f, 0.1f, 1.0f };

    DirectX::SimpleMath::Vector3 initialPosition = { 1.0, 0.0, 0.0 };
    float localRotationSpeed = 0.0;
    float parentRotationSpeed = 2.0;
    DirectX::SimpleMath::Vector3 localRotationAxis = { 0.0, 1.0, 1.0 };
    DirectX::SimpleMath::Vector3 parentRotationAxis = { 0.0, 0.0, -1.0 };

    mPlanets[4] = new Planet(
      mPlanets[3],
      new CubeRenderItem(mRenderContext, scale, color),
      initialPosition,
      localRotationSpeed,
      parentRotationSpeed,
      localRotationAxis,
      parentRotationAxis);
  }

  mGameComponents.push_back(mCamera);
  mGameComponents.push_back(mPlanets[0]);
  mGameComponents.push_back(mPlanets[1]);
  mGameComponents.push_back(mPlanets[2]);
  mGameComponents.push_back(mPlanets[3]);
  mGameComponents.push_back(mPlanets[4]);

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
