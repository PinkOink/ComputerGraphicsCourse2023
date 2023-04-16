#include "KatamariDamacyGame.h"

#include <SphereRenderItem.h>
#include <MeshRenderItem.h>


KatamariDamacyGame::KatamariDamacyGame()
{
}

KatamariDamacyGame::~KatamariDamacyGame()
{
}

bool KatamariDamacyGame::createGameComponents()
{
  mInputDevice->MouseMove.AddLambda([this](const InputDevice::MouseMoveEventArgs& args) { this->processMouseMove(args); });

  {
    float playerRadius = 1.0;
    RenderItem* renderItem = new SphereRenderItem(
      mRenderContext,
      { playerRadius, playerRadius, playerRadius },
      { {0.0, 1.0, 0.0, 1.0}, { 1.0f, 1.0f, 1.0f }, 100.0 },
      L"./Resource/Katamari3dVS.hlsl",
      L"./Resource/Katamari3dPS.hlsl"
    );

    mPlayer = new PlayerComponent(
      renderItem,
      mRenderContext, 
      mWindow, 
      playerRadius, 
      mBorderX, 
      mBorderY,
      mObjects);

    mGeometry.push_back(renderItem);
  }

  {
    mLight = new DirectionalLight(mRenderContext, mPlayer);

    mGameComponents.push_back(mLight);
  }

  mGameComponents.push_back(mPlayer);

  {
    auto floorRenderItem = new CubeRenderItem(
      mRenderContext, 
      { mBorderX, mBorderY, 1.0 }, 
      { { 0.7f, 0.7f, 0.7f, 1.0f }, { 1.0f, 1.0f, 1.0f }, 50.0f },
      L"./Resource/Katamari3dVS.hlsl",
      L"./Resource/Katamari3dPS.hlsl"
    );

    floorRenderItem->setWorldMatrix(DirectX::SimpleMath::Matrix::CreateTranslation({ 0.0, 0.0, -1.0 }));

    mFloor = new RenderItemWrapper(floorRenderItem);

    mGameComponents.push_back(mFloor);
    mGeometry.push_back(floorRenderItem);
  }

  ObjectComponent* object;
  // For debug
  //{
  //  float objectRadius = 1.0f;
  //  object = new ObjectComponent(
  //    new SphereRenderItem(
  //      mRenderContext,
  //      { objectRadius, objectRadius, objectRadius },
  //      { 1.0f, 0.7f, 1.0f, 1.0f }/*,
  //      L"./Resource/Katamari3dVS.hlsl",
  //      L"./Resource/Katamari3dPS.hlsl"*/
  //  ), objectRadius, { 0.0f, 0.0f });
  //  mObjects.push_back(object);

  //  mGameComponents.push_back(object);
  //}
  {
    float objectRadius = 0.3f;
    RenderItem* renderItem = new CubeRenderItem(
      mRenderContext,
      { objectRadius, objectRadius, objectRadius },
      { { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, 1000.0 },
      L"./Resource/Katamari3dVS.hlsl",
      L"./Resource/Katamari3dPS.hlsl"
    );
    object = new ObjectComponent(
      renderItem, objectRadius, { -5.0f, -5.0f });
    mObjects.push_back(object);

    mGameComponents.push_back(object);
    mGeometry.push_back(renderItem);
  }  
  {
    float objectRadius = 1.0f;
    RenderItem* renderItem = new SphereRenderItem(
      mRenderContext,
      { objectRadius, objectRadius, objectRadius },
      { { 0.7f, 0.5f, 0.7f, 1.0f }, { 0.0f, 0.0f, 1.0f }, 2.0 },
      L"./Resource/Katamari3dVS.hlsl",
      L"./Resource/Katamari3dPS.hlsl"
    );
    object = new ObjectComponent(
      renderItem, objectRadius, {15.0f, -5.0f});
    mObjects.push_back(object);

    mGameComponents.push_back(object);
    mGeometry.push_back(renderItem);
  }  
  {
    float objectRadius = 0.5f;
    RenderItem* renderItem = new MeshRenderItem(
      mRenderContext,
      "./Models/Gun.obj",
      L"./Models/Gun.dds",
      L"./Resource/Katamari3dVS.hlsl",
      L"./Resource/Katamari3dPS.hlsl",
      { { 1.0f, 1.0f, 1.0f }, 10.0 },
      { 3.0, 3.0, 3.0 }
    );
    object = new ObjectComponent(
      renderItem, objectRadius, {-10.0f, 10.0f});
    mObjects.push_back(object);

    mGameComponents.push_back(object);
    mGeometry.push_back(renderItem);
  }
  {
    float objectRadius = 1.0f;
    RenderItem* renderItem = new MeshRenderItem(
      mRenderContext,
      "./Models/drawer.obj",
      L"./Models/drawer.dds",
      L"./Resource/Katamari3dVS.hlsl",
      L"./Resource/Katamari3dPS.hlsl",
      { { 0.5f, 0.2f, 0.2f }, 30.0 },
      { 0.4f, 0.4f, 0.4f },
      { 0.0f, -2.0f, 0.5f }
    );
    object = new ObjectComponent(
      renderItem, objectRadius, {10.0f, 10.0f});
    mObjects.push_back(object);

    mGameComponents.push_back(object);
    mGeometry.push_back(renderItem);
  }
  {
    float objectRadius = 0.7f;
    RenderItem* renderItem = new MeshRenderItem(
      mRenderContext,
      "./Models/10200_Pineapple_v1-L2.obj",
      L"./Models/10200_Pineapple.dds",
      L"./Resource/Katamari3dVS.hlsl",
      L"./Resource/Katamari3dPS.hlsl",
      { { 0.0f, 0.0f, 1.0f }, 4.0 },
      { 0.1f * objectRadius, 0.1f * objectRadius, 0.1f * objectRadius },
      { 0.0, 0.0, -10.0 }
    );
    object = new ObjectComponent(
      renderItem, objectRadius, {-5.0f, 0.0f});
    mObjects.push_back(object);

    mGameComponents.push_back(object);
    mGeometry.push_back(renderItem);
  }

  mLight->setGeometries(mGeometry);

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

  DirectX::SimpleMath::Vector2 playerMoveDir = { 0.0, 0.0 };
  if (mInputDevice->IsKeyDown(Keys::W)) {
    playerMoveDir.x += +1.0;
  }
  if (mInputDevice->IsKeyDown(Keys::S)) {
    playerMoveDir.x += -1.0;
  }
  if (mInputDevice->IsKeyDown(Keys::D)) {
    playerMoveDir.y += +1.0;
  }
  if (mInputDevice->IsKeyDown(Keys::A)) {
    playerMoveDir.y += -1.0;
  }
  mPlayer->addMove(playerMoveDir);
}

void KatamariDamacyGame::processMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
  mPlayer->rotateCamera(args.Offset);
}
