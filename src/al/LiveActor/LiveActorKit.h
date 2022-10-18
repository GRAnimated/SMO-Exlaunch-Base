#pragma once

#include "al/PlayerHolder/PlayerHolder.h"
#include "al/area/AreaObjDirector.h"
#include "al/camera/CameraDirector.h"
#include "al/collision/CollisionDirector.h"
#include "al/hio/HioNode.h"
#include "game/System/GameSystemInfo.h"
#include "LiveActorGroup.h"
#include "al/draw/GraphicsSystemInfo.h"

namespace al {

class ActorResourceHolder;
class ExecuteDirector;
class GravityHolder;
//class GraphicsSystemInfo;
class ModelDrawBufferUpdater;
class ExecuteAsyncExecutor;
class ModelDisplayListController;
class ClippingDirector;
class HitSensorDirector;
class ScreenPointDirector;
class ShadowDirector;
class StageSwitchDirector;
class SwitchAreaDirector;
class PadRumbleDirector;
class NatureDirector;
class ModelGroup;
class ModelDrawBufferCounter;
class ExecuteAsyncExecutorUpdate;
class ItemDirectorBase;
class DemoDirector;

class LiveActorKit : public al::HioNode { // 0xf8
public:
    unsigned char gap1[0x8];
    int iVar1; // 0x8
    al::ActorResourceHolder *mActorResourceHolder;
    al::AreaObjDirector *mAreaObjDirector;
    al::ExecuteDirector *mExecuteDirector;
    al::GravityHolder *mGravityHolder;
    al::EffectSystem *mEffectSystem;
    al::GraphicsSystemInfo *mGraphicsSystemInfo;
    al::ModelDrawBufferCounter *mModelDrawBufferCounter;
    al::ModelDrawBufferUpdater *mModelDrawBufferUpdater;
    al::ExecuteAsyncExecutorUpdate *mUpdateExectorCore1;
    al::ExecuteAsyncExecutorUpdate *mUpdateExectorCore2;
    al::ModelDisplayListController *mModelDisplayListController;
    agl::DrawContext *mDrawCtx;
    al::LiveActorGroup *field_70;
    al::CameraDirector *mCameraDirector;
    al::ClippingDirector *mClippingDirector;
    al::CollisionDirector *mCollisionDirector;
    al::ItemDirectorBase *mItemDirectorBase;
    al::PlayerHolder *mPlayerHolder;
    al::HitSensorDirector *mHitSensorDirector;
    al::ScreenPointDirector *mScreenPointDirector;
    al::ShadowDirector *mShadowDirector;
    al::StageSwitchDirector *mStageSwitchDirector;
    al::SwitchAreaDirector *mSwitchAreaDirector;
    al::LiveActorGroup *allActors;
    al::DemoDirector *mDemoDirector;
    al::GamePadSystem *mGamePadSystem;
    al::PadRumbleDirector *mPadRumbleDirector;
    al::NatureDirector *mNatureDirector;
    al::ModelGroup *mModelGroup;
};
}