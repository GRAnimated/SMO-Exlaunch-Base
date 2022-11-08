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
#include "types.h"

#define MAX_TABS 5

namespace smol {
    class PartList {

    };

    class PartDropdown {
        // amount - amount of tabs
        // loc - from top left
        // end - from bottom right
        // bg - background color
        PartDropdown(int amount, sead::Vector2<int> loc, sead::Vector2<int> end, sead::Color4f bg);
        u16 back(); // can loop around to the other side
        u16 next(); // can loop around to the other side

        u16 selected;
        u16 max;

        sead::Vector2<int> location;
        sead::Vector2<int> endLoc;
        sead::Color4f bgColor;

        sead::TextWriter *writer;
    };

    namespace DebugUtil {
        void CreateCategories();

        void drawQuad(agl::DrawContext *ctx, sead::Vector2f topLeft, sead::Vector2f bottomRight, sead::Color4f color);
        void drawQuadSize(agl::DrawContext *ctx, sead::Vector2f topLeft, sead::Vector2f size, sead::Color4f color);

        void drawTab(sead::TextWriter *tw, sead::DrawContext *ctx, sead::Vector2f topLeft, char const* label, bool selected);
    }

    class DebugVariables {
        public:
        bool enableDebugMenu = true;
        bool hideDebugMenu = false;
        HakoniwaSequence *mHakoniwaSequence;
    };

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

    

    class DebugMenuMgr {
        SEAD_SINGLETON_DISPOSER(DebugMenuMgr)
        DebugMenuMgr();
        virtual ~DebugMenuMgr();
        public:
        void init(sead::DrawContext *ctx, sead::Viewport *view);
        void update();
        bool isHidden() {return vars.hideDebugMenu;};
        bool isDraw() {return vars.enableDebugMenu;};

        DebugVariables vars;
        sead::PtrArray<Category> categories;
        int selCategory = 1;
        int selPage = 1;

        PartDropdown *mainDropdown;
        PartDropdown *subDropdown;

        sead::DrawContext *mDrawContext;
        sead::Viewport *mViewport;
        sead::TextWriter *tw;
    };
}