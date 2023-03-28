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

  //auto gunRenderItem = new MeshRenderItem(
  //  mRenderContext,
  //  "./Models/Gun.obj",
  //  L"./Resource/Katamari3dVS.hlsl",
  //  L"./Resource/Katamari3dPS.hlsl"
  //);

  {
    float playerRadius = 1.0;
    mPlayer = new PlayerComponent(
      new SphereRenderItem(
        mRenderContext,
        { playerRadius, playerRadius, playerRadius },
        { 0.0, 1.0, 0.0, 1.0 }/*,
        L"./Resource/Katamari3dVS.hlsl",
        L"./Resource/Katamari3dPS.hlsl"*/
      ),
      mRenderContext, 
      mWindow, 
      playerRadius, 
      mBorderX, 
      mBorderY,
      mObjects);
    mGameComponents.push_back(mPlayer);
  }

  {
    auto floorRenderItem = new CubeRenderItem(
      mRenderContext, 
      { mBorderX, mBorderY, 1.0 }, 
      { 1.0, 1.0, 1.0, 1.0 }/*,
      L"./Resource/Katamari3dVS.hlsl",
      L"./Resource/Katamari3dPS.hlsl"*/
    );

    floorRenderItem->setWorldMatrix(DirectX::SimpleMath::Matrix::CreateTranslation({ 0.0, 0.0, -1.0 }));

    mFloor = new RenderItemWrapper(floorRenderItem);

    mGameComponents.push_back(mFloor);
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
    object = new ObjectComponent(
      new CubeRenderItem(
        mRenderContext, 
        { objectRadius, objectRadius, objectRadius }, 
        { 1.0f, 0.7f, 0.0f, 1.0f }/*,
        L"./Resource/Katamari3dVS.hlsl",
        L"./Resource/Katamari3dPS.hlsl"*/
    ), objectRadius, { -5.0f, -5.0f });
    mObjects.push_back(object);

    mGameComponents.push_back(object);
  }  
  {
    float objectRadius = 1.0f;
    object = new ObjectComponent(
      new SphereRenderItem(
        mRenderContext, 
        { objectRadius, objectRadius, objectRadius }, 
        { 1.0f, 0.7f, 1.0f, 1.0f }/*, 
        L"./Resource/Katamari3dVS.hlsl",
        L"./Resource/Katamari3dPS.hlsl"*/
      ), objectRadius, {15.0f, -5.0f});
    mObjects.push_back(object);

    mGameComponents.push_back(object);
  }  
  {
    float objectRadius = 0.5f;
    object = new ObjectComponent(
      new MeshRenderItem(
        mRenderContext,
        "./Models/Gun.obj",
        L"./Models/Gun.dds",
        L"./Resource/Katamari3dVS.hlsl",
        L"./Resource/Katamari3dPS.hlsl",
        { 3.0, 3.0, 3.0 }
      ), objectRadius, {-10.0f, 10.0f});
    mObjects.push_back(object);

    mGameComponents.push_back(object);
  }
  {
    float objectRadius = 1.0f;
    object = new ObjectComponent(
      new MeshRenderItem(
        mRenderContext,
        "./Models/drawer.obj",
        L"./Models/drawer.dds",
        L"./Resource/Katamari3dVS.hlsl",
        L"./Resource/Katamari3dPS.hlsl",
        { 0.4, 0.4, 0.4 },
        { 0.0, -2.0, 0.5 }
      ), objectRadius, {10.0f, 10.0f});
    mObjects.push_back(object);

    mGameComponents.push_back(object);
  }
  {
    float objectRadius = 0.7f;
    object = new ObjectComponent(
      new MeshRenderItem(
        mRenderContext,
        "./Models/10200_Pineapple_v1-L2.obj",
        L"./Models/10200_Pineapple.dds",
        L"./Resource/Katamari3dVS.hlsl",
        L"./Resource/Katamari3dPS.hlsl",
        { 0.1f * objectRadius, 0.1f * objectRadius, 0.1f * objectRadius },
        { 0.0, 0.0, -10.0 }
      ), objectRadius, {-5.0f, 0.0f});
    mObjects.push_back(object);

    mGameComponents.push_back(object);
  }

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