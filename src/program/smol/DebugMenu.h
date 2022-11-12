#pragma once

#include "container/seadSafeArray.h"
#include "game/HakoniwaSequence/HakoniwaSequence.h"
#include "gfx/seadColor.h"
#include "gfx/seadTextWriter.h"
#include "math/seadVector.h"
#include "container/seadPtrArray.h"
#include "basis/seadNew.h"
#include "heap/seadDisposer.h"
#include "container/seadSafeArray.h"
#include "agl/DrawContext.h"
#include "al/util.hpp"
#include "al/actor/ActorCameraTarget.h"
#include "types.h"

#define MAX_TABS 5

namespace smol {

    namespace DebugUtil {
        void CreateCategories();

        void drawQuad(agl::DrawContext *ctx, sead::Vector2f topLeft, sead::Vector2f bottomRight, sead::Color4f color);
        void drawQuadSize(agl::DrawContext *ctx, sead::Vector2f topLeft, sead::Vector2f size, sead::Color4f color);

        void drawTab(sead::TextWriter *tw, sead::DrawContext *ctx, sead::Vector2f topLeft, char const* label, bool selected);
    }

    class Page {
        public:
        Page(int ind, char const *n) {index = ind; name = n;};
        virtual void draw();
        int index;
        char const *name;
        sead::TextWriter *mw;
    };

    class Category {
        public:
        Category(int ind, char const *n) {index = ind; name = n;};
        void draw();

        int index;
        char const *name;

        sead::PtrArray<Page> pages;
    };

    class DebugLookAtCamera : public al::CameraPoser {
        public:
        DebugLookAtCamera(char const*);
        virtual void loadParam(al::ByamlIter const&) override;
        virtual void start(al::CameraStartInfo const&) override;
        virtual void init() override;
        void reset(void);
        virtual void update(void) override;
        virtual void movement(void) override;

        sead::Vector3f mPrevTargetDir = sead::Vector3f::ey;

        sead::Vector3f mStartPos = sead::Vector3f::zero;
        sead::Vector3f mStartTarget = sead::Vector3f::zero;
        float mStartDist = 0.0f;
        float mStartFOV = 35.0f;
        float speed = 50.0f;
        bool frozen = false;
    };

    class DebugMenuInputMgr {
        public:
        bool mEnableDebugMenu = true;
        bool mHideDebugMenu = false;
        bool mDisablePlayerInput = false;
    };

    class DebugMenuMgr {
        SEAD_SINGLETON_DISPOSER(DebugMenuMgr)
        DebugMenuMgr();
        virtual ~DebugMenuMgr();
        public:
        void init(sead::DrawContext *ctx, sead::Viewport *view);
        void initCamera(StageScene *stageScene);
        void update();
        bool isHidden() {return input.mHideDebugMenu;};
        bool isEnable() {return input.mEnableDebugMenu;};

        DebugMenuInputMgr input;
        sead::PtrArray<Category> categories;
        int selCategory = 1;
        int selPage = 1;

        HakoniwaSequence *mHakoniwaSequence;
        sead::DrawContext *mDrawContext;
        sead::Viewport *mViewport;
        sead::TextWriter *tw;

        al::ActorCameraTarget *mCameraTarget;
        al::CameraTicket *mFocusCamera;
        bool isCameraEnabled = false;
    };
}