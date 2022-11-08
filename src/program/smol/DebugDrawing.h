#pragma once

#include "agl/DrawContext.h"
#include "agl/TextureData.h"
#include "agl/utl.h"
#include "sead/gfx/seadViewport.h"
#include "sead/gfx/seadGraphics.h"
#include "lib.hpp"

namespace smol {
    namespace Draw {
        /* DrawTextureDataToScreen

            int channel - channel of the agl::TextureData to display
            size - down/upscales from the size of the original texture. figure this one out
            location - location to draw, 0.0, 0.0 = top left

        */
        void DrawTextureDataToScreen(agl::DrawContext *ctx, agl::TextureData *texData, int channel, sead::Viewport *viewport, sead::Vector2f size, sead::Vector2f location);
    }
}