/**
 * @file RenderBuffer.h
 * @brief Defines classes that implement a render buffer.
 */

#pragma once

#include "types.h"
#include "DrawContext.h"
#include "RenderTargetColor.h"
#include "RenderTargetDepth.h"
#include "sead/math/seadBoundBox.h"
#include "sead/gfx/seadFrameBuffer.h"
#include "sead/gfx/seadColor.h"
#include "sead/math/seadVector.h"

namespace agl
{
    class RenderBuffer : public sead::FrameBuffer
    {
    public:
        RenderBuffer();
        RenderBuffer(sead::Vector2<f32> const &, sead::BoundBox2<f32> const &);
        RenderBuffer(sead::Vector2<f32> const &, f32, f32, f32, f32);
        virtual ~RenderBuffer();

        virtual void copyToDisplayBuffer(sead::DrawContext *, sead::DisplayBuffer const *) const;
        virtual void clear(sead::DrawContext *, u32, sead::Color4f const &, f32, u32) const = 0;
        virtual void bindImpl_(sead::DrawContext *) const;

        void initialize_();
        void setRenderTargetColorNullAll();
        void adjustPhysicalAreaAndVirtualSizeFromColorTarget(u32);
        void invalidateGPUCache(agl::DrawContext *) const;
        void bind_(agl::DrawContext *, u16) const;

        agl::RenderTargetColor *_8;
        agl::RenderTargetColor *_10;
        agl::RenderTargetColor *_18;
        agl::RenderTargetColor *_20;
        agl::RenderTargetColor *_28;
        agl::RenderTargetColor *_30;
        agl::RenderTargetColor *_38;
        agl::RenderTargetColor *_40;
        agl::RenderTargetColor *_48;
        agl::RenderTargetColor *_50;
        agl::RenderTargetColor *_58;
        agl::RenderTargetDepth *_60;
    };
};