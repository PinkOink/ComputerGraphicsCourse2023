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