#include "SolarSystemGame.h"

#include "CubeRenderItem.h"
#include "SphereRenderItem.h"


SolarSystemGame::SolarSystemGame()
  : Game("SolarSystem")
{}

SolarSystemGame::~SolarSystemGame()
{
}

bool SolarSystemGame::createGameComponents()
{
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
      new SphereRenderItem(mRenderContext, scale, color),
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
    float localRotationSpeed = 3.0;
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
      new SphereRenderItem(mRenderContext, scale, color),
      initialPosition,
      localRotationSpeed,
      parentRotationSpeed,
      localRotationAxis,
      parentRotationAxis);
  }

  {    
    DirectX::SimpleMath::Vector3 scale = { 0.4f, 0.2f, 1.1f };
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

    DirectX::SimpleMath::Vector3 initialPosition = { 2.0, 0.0, 0.0 };
    float localRotationSpeed = 0.0;
    float parentRotationSpeed = 6.0;
    DirectX::SimpleMath::Vector3 localRotationAxis = { 0.0, 1.0, 1.0 };
    DirectX::SimpleMath::Vector3 parentRotationAxis = { 0.0, 1.0, 0.0 };

    mPlanets[4] = new Planet(
      mPlanets[0],
      new SphereRenderItem(mRenderContext, scale, color),
      initialPosition,
      localRotationSpeed,
      parentRotationSpeed,
      localRotationAxis,
      parentRotationAxis);
  }

  {    
    DirectX::SimpleMath::Vector3 scale = { 0.1f, 0.1f, 0.1f };
    DirectX::SimpleMath::Vector4 color = { 0.2f, 1.0f, 0.1f, 1.0f };

    DirectX::SimpleMath::Vector3 initialPosition = { 0.0f, -0.6f, 0.0f };
    float localRotationSpeed = 0.0;
    float parentRotationSpeed = 0.0;
    DirectX::SimpleMath::Vector3 localRotationAxis = { 0.0, 1.0, 1.0 };
    DirectX::SimpleMath::Vector3 parentRotationAxis = { 0.0, 0.0, -1.0 };

    mPlanets[5] = new Planet(
      mPlanets[2],
      new CubeRenderItem(mRenderContext, scale, color),
      initialPosition,
      localRotationSpeed,
      parentRotationSpeed,
      localRotationAxis,
      parentRotationAxis);
  }

  {    
    DirectX::SimpleMath::Vector3 scale = { 0.1f, 0.1f, 0.1f };
    DirectX::SimpleMath::Vector4 color = { 1.0f, 0.0f, 0.0f, 1.0f };

    DirectX::SimpleMath::Vector3 initialPosition = { 1.0, 0.0, 0.0 };
    float localRotationSpeed = 0.0;
    float parentRotationSpeed = 0.0;
    DirectX::SimpleMath::Vector3 localRotationAxis = { 0.0, 1.0, 1.0 };
    DirectX::SimpleMath::Vector3 parentRotationAxis = { 0.0, 0.0, 1.0 };

    mPlanets[6] = new Planet(
      mPlanets[3],
      new CubeRenderItem(mRenderContext, scale, color),
      initialPosition,
      localRotationSpeed,
      parentRotationSpeed,
      localRotationAxis,
      parentRotationAxis);
  }

  {    
    DirectX::SimpleMath::Vector3 scale = { 0.1f, 0.1f, 0.1f };
    DirectX::SimpleMath::Vector4 color = { 0.0f, 1.0f, 0.0f, 1.0f };

    DirectX::SimpleMath::Vector3 initialPosition = { 0.0, 1.0, 0.0 };
    float localRotationSpeed = 0.0;
    float parentRotationSpeed = 0.0;
    DirectX::SimpleMath::Vector3 localRotationAxis = { 0.0, 1.0, 1.0 };
    DirectX::SimpleMath::Vector3 parentRotationAxis = { 0.0, 0.0, 1.0 };

    mPlanets[7] = new Planet(
      mPlanets[3],
      new CubeRenderItem(mRenderContext, scale, color),
      initialPosition,
      localRotationSpeed,
      parentRotationSpeed,
      localRotationAxis,
      parentRotationAxis);
  }

  {    
    DirectX::SimpleMath::Vector3 scale = { 0.1f, 0.1f, 0.1f };
    DirectX::SimpleMath::Vector4 color = { 0.0f, 0.0f, 1.0f, 1.0f };

    DirectX::SimpleMath::Vector3 initialPosition = { 0.0, 0.0, 1.0 };
    float localRotationSpeed = 0.0;
    float parentRotationSpeed = 0.0;
    DirectX::SimpleMath::Vector3 localRotationAxis = { 0.0, 1.0, 1.0 };
    DirectX::SimpleMath::Vector3 parentRotationAxis = { 0.0, 0.0, 1.0 };

    mPlanets[8] = new Planet(
      mPlanets[3],
      new CubeRenderItem(mRenderContext, scale, color),
      initialPosition,
      localRotationSpeed,
      parentRotationSpeed,
      localRotationAxis,
      parentRotationAxis);
  }

  mCamera = new Camera(mRenderContext, mWindow);
  mCameraOrbit = new CameraOrbit(mRenderContext, mWindow, mPlanets[mCurPlanet]);
  mCameraOrbit->deactivate();

  mGameComponents.push_back(mCamera);
  mGameComponents.push_back(mCameraOrbit);
  mGameComponents.push_back(mPlanets[0]);
  mGameComponents.push_back(mPlanets[1]);
  mGameComponents.push_back(mPlanets[2]);
  mGameComponents.push_back(mPlanets[3]);
  mGameComponents.push_back(mPlanets[4]);
  mGameComponents.push_back(mPlanets[5]);
  mGameComponents.push_back(mPlanets[6]);
  mGameComponents.push_back(mPlanets[7]);
  mGameComponents.push_back(mPlanets[8]);

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

  if (mInputDevice->IsKeyDown(Keys::P)) {
    mCamera->setPerspectiveProjection();
    mCameraOrbit->setPerspectiveProjection();
  }
  if (mInputDevice->IsKeyDown(Keys::O)) {
    mCamera->setOrthographicProjection();
    mCameraOrbit->setOrthographicProjection();
  }

  if (mInputDevice->IsKeyDown(Keys::D1)) {
    mCamera->activate();
    mCameraOrbit->deactivate();
  }
  if (mInputDevice->IsKeyDown(Keys::D2)) {
    mCamera->deactivate();
    mCameraOrbit->activate();
  }

  if (mInputDevice->IsKeyDown(Keys::Left)) {
    mLeftArrowPressed = true;
  }
  if (mInputDevice->IsKeyDown(Keys::Right)) {
    mRightArrowPressed = true;
  }
  if (!mInputDevice->IsKeyDown(Keys::Left) && mLeftArrowPressed) {
    mLeftArrowPressed = false;
    mCameraOrbit->setPlanet(mPlanets[mCurPlanet = (mCurPlanet > 0) ? (mCurPlanet - 1) : (mPlanetsNum - 1)]);
  }
  if (!mInputDevice->IsKeyDown(Keys::Right) && mRightArrowPressed) {
    mRightArrowPressed = false;
    mCameraOrbit->setPlanet(mPlanets[mCurPlanet = (mCurPlanet + 1) % mPlanetsNum]);
  }

  if (mInputDevice->IsKeyDown(Keys::L)) {
    mCameraOrbit->useLocalTransform();
  }
  if (mInputDevice->IsKeyDown(Keys::K)) {
    mCameraOrbit->notUseLocalTransform();
  }
}

void SolarSystemGame::processMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
  mCamera->rotateCamera(args.Offset);
  mCameraOrbit->rotateCamera(args.Offset);
}
