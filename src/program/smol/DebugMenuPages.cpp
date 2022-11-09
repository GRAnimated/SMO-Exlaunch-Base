#include "DebugMenuPages.h"
#include "al/LiveActor/LiveActorKit.h"

#include "al/sensor/HitSensor.h"
#include "al/util/InputUtil.h"
#include "game/System/Application.h"
#include "al/scene/Scene.h"
#include "al/draw/ModelKeeper.h"

#include "al/actor/ActorCameraTarget.h"

#include "sead/gfx/seadPrimitiveRenderer.h"
#include "smol/DebugMenu.h"


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

#include <typeinfo>

int selection = 0;
int showAmount = 18;
bool enableCam = false;

void p::WorldActors::draw() {
    auto inst = smol::DebugMenuMgr::instance();
    auto tw = inst->tw;

    //tw->printf("Random Text\n");
    //tw->setScaleFromFontHeight(20.f);
    //tw->printf("Do you recognize this font?\n");

    auto leftStick = al::getLeftStick(-1);
    int scrollSpeed = leftStick.y*10;

    auto scene = inst->vars.mHakoniwaSequence->curScene;
    if (scene) {
        auto kit = scene->mSceneLiveActorKit;
        if (kit) {

            smol::DebugUtil::drawQuadSize((agl::DrawContext *)inst->mDrawContext, sead::Vector2f(210.f, 358.f), sead::Vector2f(260.f, 354.f), sead::Color4f(0.f, 0.f, 0.f, 1.f));
            tw->beginDraw();
            tw->setCursorFromTopLeft(sead::Vector2f(210.f, 338.f));
            tw->setScaleFromFontHeight(20.f);
            tw->mColor = sead::Color4f::cWhite;

            auto group = kit->allActors;
            selection += scrollSpeed;
            if (selection < 0) selection = 0;

            tw->printf("%i, %i, %i\n", scrollSpeed, selection, group->mActorCount);

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

            for (int i = 0; i < showAmount && i < group->mActorCount; i++) {
                //if (i == selection) {
                //    //tw->mColor = sead::Color4f::cBlack;
                //    smol::DebugUtil::drawQuadSize((agl::DrawContext *)inst->mDrawContext, sead::Vector2f(210.f, 358.f+(20*i)), sead::Vector2f(250.f, 20.f), sead::Color4f(.2f, .2f, .2f, 1.f));
                //    tw->endDraw();
                //    tw->beginDraw();
                //}
                if (selection+i < actorCount) {
                    tw->printf("%s\n", group->mActors[selection+i]->mActorName);
                } else {
                    selection -= i;
                }
                
                //tw->printf("%s\n", al::getModelName(group->mActors[i+scrollPos]));
            }

            tw->endDraw();

            auto actor = group->mActors[selection];
            if (actor->mModelKeeper) {
                auto prim = sead::PrimitiveRenderer::instance();
                sead::LookAtCamera *cam = al::getLookAtCamera(scene, 0);
                sead::Projection *projection = al::getProjectionSead(scene, 0);
                prim->mDrawer.setDrawContext(inst->mDrawContext);
                prim->setCamera(*cam);
                prim->setProjection(*projection);
                prim->setModelMatrix(sead::Matrix34f::ident);

                prim->begin();
                prim->drawSphere8x16(al::getTrans(actor), 200.0f, sead::Color4f(0.f, 0.f, 1.f, .6f));
                prim->end();

                smol::DebugUtil::drawQuadSize((agl::DrawContext *)inst->mDrawContext, sead::Vector2f(474.f, 352.f), sead::Vector2f(330.f, 30.f), sead::Color4f(1.f, 1.f, 1.f, 1.f));
                tw->beginDraw();
                tw->setCursorFromTopLeft(sead::Vector2f(479.f, 354.f));
                tw->setScaleFromFontHeight(28.f);
                tw->mColor = sead::Color4f::cBlack;
                char const* modelName = al::getModelName(actor);
                if (actor->mModelKeeper && modelName) tw->printf("%s\n", modelName);
                
                tw->endDraw();

                //al::ActorCameraTarget *target = al::createActorCameraTarget(actor, 1000.0f);

                if (al::isPadTriggerY(-1)) {
                    //al::setFixActorCameraTarget(inst->mFocusCamera, actor);
                    
                    inst->mFocusCamera->mPoser->mAt = al::getTrans(actor);
                    al::startCamera(scene, inst->mFocusCamera, 10);
                }
                if (al::isPadTriggerX(-1)) {
                    al::endCamera(scene, inst->mFocusCamera, 10, true);
                }
            }

            tw->setScaleFromFontHeight(20.f);
        }
    }
}