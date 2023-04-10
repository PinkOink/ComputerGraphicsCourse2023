#pragma once

#include <Game.h>
#include <CubeRenderItem.h>

#include "PlayerComponent.h"
#include "ObjectComponent.h"
#include "DirectionalLight.h"


class RenderItemWrapper : public GameComponent
{
public:
  RenderItemWrapper(CubeRenderItem* item)
    : mItem(item)
  {}

  virtual bool init() { return true; };
  virtual bool update(float deltaTime) { return true; };
  virtual bool updateSubresources() { return mItem->updateSubresources(); };
  virtual bool draw() { return mItem->draw(); };

  virtual ~RenderItemWrapper() { delete mItem; };

protected:
  RenderItem* mItem;
};


class KatamariDamacyGame : public Game
{
public:
  KatamariDamacyGame();

  virtual ~KatamariDamacyGame();

protected:
  KatamariDamacyGame(const KatamariDamacyGame& rhs) = delete;
  KatamariDamacyGame(KatamariDamacyGame&& rhs) = delete;
  KatamariDamacyGame& operator=(const KatamariDamacyGame& rhs) = delete;
  KatamariDamacyGame& operator=(KatamariDamacyGame&& rhs) = delete;

  virtual bool createGameComponents() override;

  virtual void processInputDevice() override;
  virtual void processMouseMove(const InputDevice::MouseMoveEventArgs& args);

protected:
  DirectionalLight* mLight = nullptr;
  PlayerComponent* mPlayer = nullptr;
  std::vector<ObjectComponent*> mObjects;
  RenderItemWrapper* mFloor = nullptr;

  const float mBorderX = 50.0f;
  const float mBorderY = 50.0f;
};

