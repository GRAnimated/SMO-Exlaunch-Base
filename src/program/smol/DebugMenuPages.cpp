#include "DebugMenuPages.h"
#include "al/LiveActor/LiveActorKit.h"

#include "al/util/InputUtil.h"
#include "game/System/Application.h"
#include "al/scene/Scene.h"


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

int scrollPos = 0;

void p::WorldActors::draw() {
    auto inst = smol::DebugMenuMgr::instance();
    auto tw = inst->tw;

    tw->printf("Random Text\n");
    tw->setScaleFromFontHeight(20.f);
    tw->printf("Do you recognize this font?\n");

    auto leftStick = al::getLeftStick(-1);
    int scrollSpeed = leftStick.y*10;

    auto scene = inst->vars.mHakoniwaSequence->curScene;
    if (scene) {
        auto kit = scene->mSceneLiveActorKit;
        if (kit) {
            auto group = kit->allActors;
            scrollPos += scrollSpeed;
            if (scrollPos < 0) scrollPos = 0;
            for (int i = 0; i < scrollPos + 15 && i < group->mActorCount; i++) {
                //tw->printf("%s\n", group->mActors[i+scrollPos]->mActorName);
                tw->printf("%s\n", al::getModelName(group->mActors[i+scrollPos]));
            }
        }
    }
}