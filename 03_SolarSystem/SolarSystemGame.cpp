#include "SolarSystemGame.h"


SolarSystemGame::SolarSystemGame()
  : Game("SolarSystem")
{}

SolarSystemGame::~SolarSystemGame()
{}

bool SolarSystemGame::createGameComponents()
{
  mCamera = new Camera(mRenderContext, mWindow);
  mCube = new CubeRenderItem(mRenderContext);

  mInputDevice->MouseMove.AddLambda([this](const InputDevice::MouseMoveEventArgs& args) { this->processMouseMove(args); });

  class CubeWrapper : public GameComponent
  {
  public:
    CubeWrapper(CubeRenderItem* cube)
      : mCube(cube)
    {}

    virtual bool init() { return true; };
    virtual bool update(float deltaTime) { return true; };
    virtual bool updateSubresources() {
      mCube->updateSubresources();
      return true;
    };
    virtual bool draw() {
      mCube->draw();
      return true;
    };

    virtual ~CubeWrapper() = default;

  protected:
    CubeRenderItem* mCube;
  };

  mGameComponents.push_back(mCamera);
  mGameComponents.push_back(new CubeWrapper(mCube));

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
