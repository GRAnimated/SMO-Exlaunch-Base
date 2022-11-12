#include "DebugMenuPages.h"
#include "al/LiveActor/LiveActorKit.h"

#include "al/camera/alCameraPoserFunction.h"
#include "al/sensor/HitSensor.h"
#include "al/util.hpp"
#include "al/util/InputUtil.h"
#include "al/util/LiveActorUtil.h"
#include "game/System/Application.h"
#include "al/scene/Scene.h"
#include "al/draw/ModelKeeper.h"

#include "al/actor/ActorCameraTarget.h"
#include "al/factory/CameraPoserFactoryEntries100.h"

#include "gfx/seadColor.h"
#include "math/seadBoundBox.h"
#include "sead/gfx/seadPrimitiveRenderer.h"
#include "smol/DebugMenu.h"

#include "TypeInfo.h"


#define MAX_CATEGORIES 5

/*
Menus:
Info
    -Info
    -Stats
Player
    -Info
    -Anims
    -Demo
Camera
    -Info
    -Freecam
Cheats
    -Player
    -World
World
    -Actors
    -Scenario
    -Graphics
    -Physics
    -Demo
System
    -Audio
    -Input
Misc
*/

sead::PtrArray<smol::Category> CreatePages() {
    sead::PtrArray<smol::Category> categories;
    categories.tryAllocBuffer(MAX_CATEGORIES, nullptr);



    CreateCategory(1, 5, Info)
    page(1, Info, Info, p::InfoInfo)
    page(2, Info, Stats, p::InfoStats)

    CreateCategory(2, 5, Player)
    page(1, Player, Info, p::Template)
    page(2, Player, Anims, p::Template)
    page(3, Player, Demo, p::Template)

    CreateCategory(3, 5, Camera)
    page(1, Camera, Info, p::Template)
    page(2, Camera, Freecam, p::Template)

    CreateCategory(4, 5, Cheats)
    page(1, Cheats, Player, p::Template)
    page(2, Cheats, World, p::Template)

    CreateCategory(5, 5, World)
    page(1, World, Actors, p::WorldActors)
    page(2, World, Scenario, p::Template)
    page(3, World, Graphics, p::Template)
    page(4, World, Physics, p::Template)
    page(5, World, Demo, p::Template)

    CreateCategory(6, 5, System)
    page(1, System, Audio, p::Template)
    page(2, System, Input, p::Template)

    return categories;
};

void smol::Category::draw() {
    for (int i = 0; i < pages.size(); i++) {
        auto tw = smol::DebugMenuMgr::instance()->tw;
        tw->mColor = sead::Color4f(1.f, 1.f, 1.f, 1.f);
        tw->beginDraw();
        tw->setCursorFromTopLeft(sead::Vector2f(210.f, 350.f));
        tw->setScaleFromFontHeight(20.f);

        if (pages[i]->index == smol::DebugMenuMgr::instance()->selPage) pages[i]->draw();

        tw->endDraw();
    }
}

void smol::Page::draw() {

}

void p::Template::draw() {
    auto tw = smol::DebugMenuMgr::instance()->tw;

    tw->printf("TEMPLATE PAGE\n");
}

void p::InfoInfo::draw() {
    auto tw = smol::DebugMenuMgr::instance()->tw;

    tw->setScaleFromFontHeight(32.f);
    tw->printf("SMO Project Template\n");
    tw->setScaleFromFontHeight(20.f);
    tw->printf("ZL - ZR switches categories\n");
    tw->printf(" L -  R switches pages\n");

    tw->printf("test text aaaaaa\n");

}

void p::InfoStats::draw() {
    auto tw = smol::DebugMenuMgr::instance()->tw;

    tw->setScaleFromFontHeight(20.f);
    tw->printf("FPS: %d\n", static_cast<int>(round(Application::instance()->mFramework->calcFps())));

    auto sequenceHeap = al::getSequenceHeap();
    auto sceneResourceHeap = al::getSceneResourceHeap();
    auto sceneHeap = al::getSceneHeap();
    auto worldResourceHeap = al::getWorldResourceHeap();
    if (sequenceHeap)      tw->printf("     SequenceHeap Free Size: %.2f/%.2f\n", sequenceHeap->getFreeSize() * 0.001f, sequenceHeap->getSize() * 0.001f);
    if (sceneResourceHeap) tw->printf("SceneResourceHeap Free Size: %.2f/%.2f\n", sceneResourceHeap->getFreeSize() * 0.001f, sceneResourceHeap->getSize() * 0.001f);
    if (sceneHeap)         tw->printf("        SceneHeap Free Size: %.2f/%.2f\n", sceneHeap->getFreeSize() * 0.001f, sceneHeap->getSize() * 0.001f);
    if (worldResourceHeap) tw->printf("WorldResourceHeap Free Size: %.2f/%.2f\n", worldResourceHeap->getFreeSize() * 0.001f, worldResourceHeap->getSize() * 0.001f);

}

void p::WorldActors::drawActorInfo(al::LiveActor *actor) {
    auto inst = smol::DebugMenuMgr::instance();
    auto scene = inst->mHakoniwaSequence->curScene;
    auto tw = inst->tw;

    if (actor->mModelKeeper) {

        auto prim = sead::PrimitiveRenderer::instance();
        sead::LookAtCamera *cam = al::getLookAtCamera(scene, 0);
        sead::Projection *projection = al::getProjectionSead(scene, 0);
        prim->mDrawer.setDrawContext(inst->mDrawContext);
        prim->setCamera(*cam);
        prim->setProjection(*projection);
        prim->setModelMatrix(sead::Matrix34f::ident);
        prim->begin();
        sead::BoundBox3f bounds;
        al::calcModelBoundingBox(&bounds, actor);
        //auto cube = sead::PrimitiveDrawer::CubeArg(sead::Vector3f::zero, sead::Vector3f(200.f, 200.f, 200.f), sead::Color4f(0.f, 0.f, 1.f, .6f));
        auto cube = sead::PrimitiveDrawer::CubeArg(bounds, sead::Color4f(0.f, 0.f, 1.f, .5f));
        //prim->drawCube(cube);
        //prim->setModelMatrix(sead::Matrix34f::ident);
        sead::Matrix34f rotation = sead::Matrix34f::ident;
        al::makeMtxRT(&rotation, actor);
        rotation.setTranslation(al::getTrans(actor));
        prim->setModelMatrix(rotation);
        prim->drawSphere8x16(sead::Vector3f::zero, 30.0f, sead::Color4f::cRed);
        prim->drawCube(cube);
        //prim->drawSphere8x16(al::getTrans(actor), 200.0f, sead::Color4f(0.f, 0.f, 1.f, .6f));
        prim->end();
        
        //al::ActorCameraTarget *target = al::createActorCameraTarget(actor, 1000.0f);
        if (al::isPadTriggerX(-1)) inst->isCameraEnabled = !inst->isCameraEnabled;
        inst->mCameraTarget->actor = actor;
        if (inst->isCameraEnabled) {
            al::setCameraTarget(scene, inst->mCameraTarget);
            al::startCamera(scene, inst->mFocusCamera, 0);
            auto cam = (al::CameraPoserFollowSimple *)inst->mFocusCamera->mPoser;
            cam->mDistance = al::powerIn(bounds.getSizeX()+bounds.getSizeY()+bounds.getSizeZ() + 500.f, 1.02f);
            cam->mOffsetY = bounds.getSizeY()/2;
            cam->mIsRotateH = true;
            //alCameraPoserFunction::offVerticalAbsorb(inst->mFocusCamera->mPoser);
        } else {
            al::resetCameraTarget(scene, inst->mCameraTarget);
            al::endCamera(scene, inst->mFocusCamera, 0, false);
        }
    }

    char const* modelName = "";
    if (actor->mModelKeeper) modelName = al::getModelName(actor);
    auto typeInfo = TypeInfo::getClassTypeInfo(actor)->getName();

    int chrSize = sead::calcStrLength_(modelName)+sead::calcStrLength_(typeInfo);

    smol::DebugUtil::drawQuadSize((agl::DrawContext *)inst->mDrawContext, sead::Vector2f(474.f, 352.f), sead::Vector2f(14.f*chrSize + 10.0f, 30.f), sead::Color4f(1.f, 1.f, 1.f, 1.f));
    tw->beginDraw();
    tw->setCursorFromTopLeft(sead::Vector2f(479.f, 354.f));
    tw->setScaleFromFontHeight(28.f);
    tw->mColor = sead::Color4f::cBlack;

    tw->printf("%s  ", modelName);
    tw->mColor = sead::Color4f::cBlue;
    tw->printf("%s\n", typeInfo);

    tw->setScaleFromFontHeight(18.f);
    tw->mColor = sead::Color4f::cWhite;
    tw->setCursorFromTopLeft(sead::Vector2f(474.f, 384.f));

    auto pk = actor->mPoseKeeper;

    if (pk) {
        auto rot = pk->getRotate();
        auto scale = pk->getScale();
        auto velocity = pk->getVelocity();
        auto front = pk->getFront();
        auto up = pk->getUp();
        auto quat = pk->getQuat();
        auto grav = pk->getGravity();

        int posKeep = tw->posX;
    
        tw->printf("  Pos: %f, %f, %f\n", pk->mTranslation.x, pk->mTranslation.y, pk->mTranslation.z); 
        tw->posX = posKeep;
        if (pk->getRotatePtr())     tw->printf("  Rot: %.2f, %.2f, %.2f\n", rot.x, rot.y, rot.z);
        tw->posX = posKeep;
        if (pk->getScalePtr())      tw->printf("  Scl: %.2f, %.2f, %.2f\n", scale.x, scale.y, scale.z);
        tw->posX = posKeep;
        if (pk->getVelocityPtr())   tw->printf("  Vel: %.2f, %.2f, %.2f\n", velocity.x, velocity.y, velocity.z);
        tw->posX = posKeep;
        if (pk->getFrontPtr())      tw->printf("Front: %.2f, %.2f, %.2f\n", front.x, front.y, front.z);
        tw->posX = posKeep;
        if (pk->getUpPtr())         tw->printf("   Up: %.2f, %.2f, %.2f\n", up.x, up.y, up.z);
        tw->posX = posKeep;
        if (pk->getQuatPtr())       tw->printf(" Quat: %.2f, %.2f, %.2f, %.2f\n", quat.x, quat.y, quat.z, quat.w);
        tw->posX = posKeep;
        if (pk->getGravityPtr())    tw->printf(" Grav: %.2f, %.2f, %.2f\n", grav.x, grav.y, grav.z);
        tw->posX = posKeep;

    }

    tw->setCursorFromTopLeft(sead::Vector2f(474.f, 384.f+(7*18)));

    //if (al::isExistAction(actor)) tw->printf("Action: %s, %.2f/%.2f, %.2ffps\n", al::getActionName(actor), al::getActionFrame(actor), al::getActionFrameMax(actor), al::getActionFrame(actor));

    tw->endDraw();
}

void p::WorldActors::draw() {
    auto inst = smol::DebugMenuMgr::instance();
    auto tw = inst->tw;

    //tw->printf("Random Text\n");
    //tw->setScaleFromFontHeight(20.f);
    //tw->printf("Do you recognize this font?\n");

    int steps = 10;

    auto leftStick = al::getLeftStick(-1);
    int scrollSpeed = -leftStick.y*steps;
    int absScrollSpeed = sead::MathCalcCommon<s32>::abs(scrollSpeed);

    auto scene = inst->mHakoniwaSequence->curScene;
    if (scene) {
        auto kit = scene->mSceneLiveActorKit;
        if (kit) {

            smol::DebugUtil::drawQuadSize((agl::DrawContext *)inst->mDrawContext, sead::Vector2f(210.f, 358.f), sead::Vector2f(260.f, 354.f), sead::Color4f(0.f, 0.f, 0.f, 1.f));
            tw->beginDraw();
            tw->setCursorFromTopLeft(sead::Vector2f(210.f, 338.f));
            tw->setScaleFromFontHeight(20.f);
            tw->mColor = sead::Color4f::cWhite;

            auto group = kit->allActors;
            wait++;
            if (scrollSpeed < 6 && wait % steps == 0 && scrollSpeed != 0) {
                if (scrollSpeed < 0) {
                    menuSelection -= 1;
                } else {
                    menuSelection += 1;
                }
            }
            if (wait % absScrollSpeed == steps-absScrollSpeed && scrollSpeed != 0) {
                if (scrollSpeed < 0) {
                    menuSelection -= 1;
                } else {
                    menuSelection += 1;
                }
                wait = 0;
            }
            //selection += scrollSpeed;
            if (menuSelection < 0) menuSelection = 0;
            if (menuSelection > group->mActorCount) menuSelection = group->mActorCount;

            tw->printf("%i, %i, %i\n", scrollSpeed, menuSelection, group->mActorCount);

            tw->setCursorFromTopLeft(sead::Vector2f(210.f, 358.f));

            //if (selection < 0) selection = 0;
            //if (selection > group->mActorCount) selection = group->mActorCount;
            //scrollPos = selection;
            //if (scrollPos > group->mActorCount - 15) scrollPos = group->mActorCount-15;
            //for (int i = 0; i < 15; i++) {
            //    if (i == selection) {
            //        //tw->mColor = sead::Color4f::cBlack;
            //        smol::DebugUtil::drawQuadSize((agl::DrawContext *)inst->mDrawContext, sead::Vector2f(210.f, 358.f+(20*i)), sead::Vector2f(250.f, 20.f), sead::Color4f(.2f, .2f, .2f, 1.f));
            //        tw->endDraw();
            //        tw->beginDraw();
            //    }
            //    tw->printf("%s\n", group->mActors[scrollPos+i]->mActorName);
            //    //tw->printf("%s\n", al::getModelName(group->mActors[i+scrollPos]));
            //}

            //    if (i == selection) {
            //        //tw->mColor = sead::Color4f::cBlack;
            //        smol::DebugUtil::drawQuadSize((agl::DrawContext *)inst->mDrawContext, sead::Vector2f(210.f, 358.f+(20*i)), sead::Vector2f(250.f, 20.f), sead::Color4f(.2f, .2f, .2f, 1.f));
            //        tw->endDraw();
            //        tw->beginDraw();
            //    }

            int actorCount = group->mActorCount;
            int extraShowAmount = 0;
            if (al::isPadTriggerLeft(-1)) mShowInMenuType += 1;
            if (mShowInMenuType > 3) mShowInMenuType = 0;

            for (int i = 0; i < showAmount+extraShowAmount && i < actorCount; i++) {

                auto actor = group->mActors[menuSelection+i];

                if (menuSelection+i < actorCount) {

                    if (i == 0) tw->mColor = sead::Color4f::cGray; // selected, TODO - make this better

                    sead::Color4f model = sead::Color4f(0.470588235f, 0.745098039, 1.f, 1.f);
                    sead::Color4f parent = sead::Color4f(1.f, 0.62745098f, 0.62745098f, 1.f);
                    sead::Color4f dead = sead::Color4f(1.f, 0.12745098f, 0.12745098f, 1.f);

                    char const *modelName = actor->mActorName;

                    if (actor->mModelKeeper) {
                        modelName = al::getModelName(actor);
                        //if (!modelName) modelName = " ";
                    }

                    if (mShowInMenuType == 0) { // Everything
                        tw->mColor = sead::Color4f::cWhite;
                        if (actor->mSubActorKeeper) tw->mColor = parent;
                        if (actor->mModelKeeper) tw->mColor = model;
                        if (al::isDead(actor)) tw->mColor = dead;
                        tw->printf("%s\n", modelName);
                    }
                    else if (mShowInMenuType == 1) { // Parent actors only
                        if (actor->mSubActorKeeper) {
                            tw->mColor = parent;
                            tw->printf("%s\n", modelName);
                        } else {
                            extraShowAmount += 1;
                        }
                    }
                    else if (mShowInMenuType == 2) { // Model actors only
                        if (actor->mModelKeeper) {
                            tw->mColor = model;
                            tw->printf("%s\n", modelName);
                        } else {
                            extraShowAmount += 1;
                        }
                    }
                    else if (mShowInMenuType == 3) { // Dead actors only
                        if (al::isDead(actor)) {
                            tw->mColor = dead;
                            tw->printf("%s\n", modelName);
                        } else {
                            extraShowAmount += 1;
                        }
                    }

                    tw->mColor = sead::Color4f::cWhite;
                } else {
                    menuSelection -= i;
                }
                
                //tw->printf("%s\n", al::getModelName(group->mActors[i+scrollPos]));
            }

            tw->endDraw();

            drawActorInfo(group->mActors[menuSelection]);
            if (al::isPadTriggerY(-1)) {
                group->mActors[menuSelection]->makeActorAlive();
            }
            if (al::isPadTriggerA(-1)) {
                group->mActors[menuSelection]->makeActorDead();
            }

            tw->setScaleFromFontHeight(20.f);
        }
    }
}