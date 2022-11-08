#include "DebugMenu.h"
#include "DebugMenuPages.h"
#include "agl/utl.h"
#include "gfx/seadTextWriter.h"
#include "nx/kernel/svc.h"
#include "al/util.hpp"

namespace smol {

    namespace DebugUtil {

        void drawQuad(agl::DrawContext *ctx, sead::Vector2f topLeft, sead::Vector2f bottomRight, sead::Color4f color) {
            //agl::utl::DevTools::beginDrawImm(ctx, sead::Matrix34<float>::ident, sead::Matrix44<float>::ident);
            //auto p1 = sead::Vector3f(topLeft.x, topLeft.y, 0.f); // top left
            //auto p2 = sead::Vector3f(bottomRight.x, topLeft.y, 0.f); // top right
            //auto p3 = sead::Vector3f(topLeft.x, bottomRight.y, 0.f); // bottom left
            //auto p4 = sead::Vector3f(bottomRight.x, bottomRight.y, 0.f); // bottom right
            //agl::utl::DevTools::drawTriangleImm(ctx, p1, p2, p3, color);
            //agl::utl::DevTools::drawTriangleImm(ctx, p2, p3, p4, color);
        }
        inline float cX(float x) {
            return x/1280.f*2.f-1.f;
        } 
        inline float cY(float x) {
            return -(x/720.f*2.f-1.f);
        } 
        void drawQuadSize(agl::DrawContext *ctx, sead::Vector2f topLeft, sead::Vector2f size, sead::Color4f color) {
            agl::utl::DevTools::beginDrawImm(ctx, sead::Matrix34<float>::ident, sead::Matrix44<float>::ident);
            auto p1 = sead::Vector3f(cX(topLeft.x),        cY(topLeft.y), 0.f); // top left
            auto p2 = sead::Vector3f(cX(topLeft.x+size.x), cY(topLeft.y), 0.f); // top right
            auto p3 = sead::Vector3f(cX(topLeft.x),        cY(topLeft.y+size.y), 0.f); // bottom left
            auto p4 = sead::Vector3f(cX(topLeft.x+size.x), cY(topLeft.y+size.y), 0.f); // bottom right
            agl::utl::DevTools::drawTriangleImm(ctx, p1, p2, p3, color);
            agl::utl::DevTools::drawTriangleImm(ctx, p2, p3, p4, color);
        }

        void drawTab(sead::TextWriter *tw, sead::DrawContext *ctx, sead::Vector2f topLeft, char const* label, bool selected) {
            sead::Color4f bC = sead::Color4f::cBlack;
            sead::Color4f tC = sead::Color4f::cWhite;
            if (selected) {
                bC = sead::Color4f::cWhite;
                tC = sead::Color4f::cBlack;
            }
            smol::DebugUtil::drawQuadSize((agl::DrawContext *)ctx, topLeft, sead::Vector2f(100.f, 20.f), bC);
            tw->beginDraw();
            tw->setScaleFromFontHeight(20.f);

            tw->mColor = tC;

            tw->setCursorFromTopLeft(topLeft);
            tw->printf("%s\n", label);

            tw->endDraw();
        }
    }

    

    PartDropdown::PartDropdown(int amount, sead::Vector2<int> loc, sead::Vector2<int> end, sead::Color4f bg) {
        max = amount;
        location = loc;
        endLoc = end;
        bgColor = bg;

        auto mgr = DebugMenuMgr::instance();
        writer = new sead::TextWriter(mgr->mDrawContext, mgr->mViewport);
    }

    DebugMenuMgr::DebugMenuMgr() {}

    void DebugMenuMgr::init(sead::DrawContext *ctx, sead::Viewport *view) {
        mDrawContext = ctx;
        mViewport = view;
        tw = new sead::TextWriter(ctx);
        tw->setupGraphics(ctx);

        categories = CreatePages();
        
    };

    void DebugMenuMgr::update() {

        if (!vars.enableDebugMenu) return;

        //svcOutputDebugString("test", 0x50);
        tw->mViewport = mViewport;
        tw->mColor = sead::Color4f(0.f, 0.f, 0.f, 1.f);
        tw->beginDraw();
        tw->setCursorFromTopLeft(sead::Vector2f(80.f, 80.f));

        tw->setScaleFromFontHeight(20.f);

        smol::DebugUtil::drawQuadSize((agl::DrawContext *)mDrawContext, sead::Vector2f(0.f, 348.f), sead::Vector2f(200.f, 372.f), sead::Color4f(1.f, 1.f, 1.f, .6f));
        smol::DebugUtil::drawQuadSize((agl::DrawContext *)mDrawContext, sead::Vector2f(200.f, 348.f), sead::Vector2f(1080.f, 372.f), sead::Color4f(0.f, 0.f, 0.f, .6f));

        if (al::isPadTriggerZL(-1)) selCategory -= 1;
        if (al::isPadTriggerZR(-1)) selCategory += 1;
        if (al::isPadTriggerL(-1)) selPage -= 1;
        if (al::isPadTriggerR(-1)) selPage += 1;

        if (selCategory < 1) selCategory = 1;
        if (selPage < 1) selPage = 1;

        for (int i = 0; i < categories.size(); i++) {
            DebugUtil::drawTab(tw, mDrawContext, sead::Vector2f(0.f, 348.f+i*20+i), categories[i]->name, categories[i]->index == selCategory);
            if (categories[i]->index == selCategory) {
                auto oops = categories[i]->pages;
                for (int o = 0; o < oops.size(); o++) {
                    DebugUtil::drawTab(tw, mDrawContext, sead::Vector2f(100.f, 348.f+o*20+o), oops[o]->name, oops[o]->index == selPage);
                }
                categories[i]->draw();
            }
        }

        tw->beginDraw();

        tw->mColor = sead::Color4f(0.f, 0.f, 0.f, 1.f);

        tw->setCursorFromTopLeft(sead::Vector2f(80.f, 80.f));
        tw->printf("BWAAAA %i, %i, %i, %i\n", selCategory, selPage, categories[0]->index, categories[1]->index);

        tw->endDraw();

        //DebugUtil::drawTab(tw, mDrawContext, sead::Vector2f(0.f, 348.f), "Info", false);
        //DebugUtil::drawTab(tw, mDrawContext, sead::Vector2f(0.f, 348.f+20.f), "World", true);

    }

    SEAD_SINGLETON_DISPOSER_IMPL(DebugMenuMgr);
}