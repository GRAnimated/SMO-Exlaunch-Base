/**
 * @file GameDataFile.h
 * @brief Utilites for agl namespace.
 */

#pragma once

#include "../types.h"
#include "DrawContext.h"
#include "sead/math/seadVector.h"
#include "sead/math/seadMatrix.h"
#include "sead/gfx/seadColor.h"
#include "sead/gfx/seadViewport.h"
#include "TextureData.h"
#include "shader/ShaderProgram.h"

namespace agl {
    struct RenderBuffer;

    //struct ShaderProgram;
    struct ShaderLocation {
        int mLocationByShaderType;
    };

    struct SamplerLocation : agl::ShaderLocation {
        SamplerLocation(sead::SafeString shader = "aaaa", int loc = -1) {shaderName = shader; mLocationByShaderType = loc;};
        void search(agl::ShaderProgram const&);
        sead::SafeString shaderName;
    };

    struct TextureSampler {
        TextureSampler();
        TextureSampler(agl::TextureData const&);
        void applyTextureData(agl::TextureData const&);
        bool activate(agl::DrawContext *,agl::ShaderLocation const&,int,bool) const;
    };
}

namespace agl::utl {
    class ImageFilter2D {
        public:
        struct Channel {
            Channel(int ch) {channel = ch;};
            int channel;
        };
        //void static drawColorQuadTriangle(agl::DrawContext *,sead::Color4f const&,float);

        void static drawTexture(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static draw(agl::DrawContext *,agl::ShaderProgram const&,agl::TextureSampler const&,sead::Viewport const&,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawTextureQuadTriangle(agl::DrawContext *,agl::TextureSampler const&);
        void static drawQuadTriangle(agl::DrawContext *,agl::ShaderProgram const&,agl::TextureSampler const&);
        void static drawDepthQuadTriangle(agl::DrawContext *,agl::TextureSampler const&);
        void static drawTextureChannel(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,agl::utl::ImageFilter2D::Channel,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawTextureMipLevel(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,float,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawTexture2DArray(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,int,sead::Vector2<float> const&,sead::Vector2<float> const&,float);
        void static drawTexture3D(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,float,sead::Vector2<float> const&,sead::Vector2<float> const&,float);
        //void static drawTextureCubeMap(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,agl::CubeMapFace,sead::Vector2<float> const&,sead::Vector2<float> const&,float);
        //void static drawTextureCubeArray(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,int,agl::CubeMapFace,sead::Vector2<float> const&,sead::Vector2<float> const&,float);
        void static drawTextureTexCoord(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,sead::Vector2<float> const&,float,sead::Vector2<float> const&,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawTextureMSAA(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,sead::Vector2<float> const&,sead::Vector2<float> const&);
        //void static drawReduce(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,agl::utl::ImageFilter2D::ReduceScale,float,sead::Vector2<float> const&);
        //void static getReduceProgram_(agl::utl::ImageFilter2D::ReduceScale);
        //void static drawBlur(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,agl::utl::ImageFilter2D::BlurType,sead::Vector2<float> const&,sead::Vector2<float> const&,sead::Vector2<float> const&);
        //void static getBlurProgram_(agl::utl::ImageFilter2D::BlurType);
        //void static drawGaussian(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,agl::utl::ImageFilter2D::GaussianKernel,bool,bool,sead::Vector2<float> const&);
        void static drawCubic(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,sead::Vector2<float> const&,float,sead::Vector2<float> const&,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawAlphaModifiedTexture(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawColorDrift(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,sead::Vector2<float> const&,sead::Vector2<float> const&,sead::Vector2<float> const&,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawNV12Decord(agl::DrawContext *,agl::TextureSampler const&,agl::TextureSampler const&,sead::Viewport const&,sead::Vector2<float> const&,sead::Vector2<float> const&,bool);
        void static drawNV12DecordQuadTriangle(agl::DrawContext *,agl::TextureSampler const&,agl::TextureSampler const&,bool);
        //void static drawCubemapGaussian(agl::DrawContext *,agl::TextureSampler const&,uint,uint,agl::utl::ImageFilter2D::BlurType,float);
        //void static drawCubemapGaussian(agl::DrawContext *,agl::TextureSampler const&,uint,uint,uint,agl::utl::ImageFilter2D::BlurType,float);
        void static drawCubemapIrradiance(agl::DrawContext *,agl::TextureSampler const&,uint,uint,sead::Vector3<float> const&);
        void static drawCubemapSH(agl::DrawContext *,agl::TextureSampler const&,float,sead::Vector2<float> const&,float,agl::TextureSampler const*,sead::Vector4<float> const&,sead::Vector4<float> const&);
        void static drawCubemapSHPoint(agl::DrawContext *,agl::TextureSampler const&,float,sead::Vector3<float> const&,float,agl::TextureSampler const*,sead::Vector4<float> const&,sead::Vector4<float> const&);
        void static drawCubemapSHOcclusion(agl::DrawContext *,agl::TextureSampler const&,float,sead::Vector2<float> const&,sead::Vector3<float> const&,float);
        void static drawFXAA(agl::DrawContext *,agl::TextureSampler const&);
        void static drawFXAA(agl::DrawContext *,agl::TextureSampler const&,agl::TextureSampler const&);
        void static drawTextureGamma(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,float,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawTextureGammaQuadTriangle(agl::DrawContext *,agl::TextureSampler const&,float);
        void static drawLinearDepth(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,float,float,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawLinearDepthArray(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,int,float,float,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawUint(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,sead::Vector4<float> const&,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawUintArray(agl::DrawContext *,agl::TextureSampler const&,sead::Viewport const&,int,sead::Vector4<float> const&,sead::Vector2<float> const&,sead::Vector2<float> const&);
        void static drawLinearDepthQuadTriangle(agl::DrawContext *,agl::TextureSampler const&,float,float);
        void static drawColorQuadTriangle(agl::DrawContext *,sead::Color4f const&,float);
        void static drawDepthMaskQuadTriangle(agl::DrawContext *,agl::TextureSampler const&);
        void static drawLuminanceQuadTriangle(agl::DrawContext *,agl::TextureSampler const&,sead::Vector3<float> const&);
        void static drawNormalMap(agl::DrawContext *,agl::TextureSampler const&,float,float);
        //void static drawBC(agl::DrawContext *,agl::TextureSampler const&,uint,uint,agl::utl::ImageFilter2D::TextureCompressType);
        //void static drawBCHQ(agl::DrawContext *,agl::TextureSampler const&,uint,uint,agl::utl::ImageFilter2D::TextureCompressType);
        void static draw2DArrayColorQuadTriangle(agl::DrawContext *,agl::TextureSampler const&,int);
        void static draw2DArrayDepthQuadTriangle(agl::DrawContext *,agl::TextureSampler const&,int);
        void static draw2DMinMaxQuadTriangle(agl::DrawContext *,agl::TextureSampler const&,bool,bool);
        void static draw2DArrayMinMaxQuadTriangle(agl::DrawContext *,agl::TextureSampler const&,bool,bool,int);
    };

    class DevTools {
        public:
        void static beginDrawImm(agl::DrawContext *,sead::Matrix34<float> const&,sead::Matrix44<float> const&);
        void static drawTriangleImm(agl::DrawContext*, sead::Vector3<float> const&, sead::Vector3<float> const&, sead::Vector3<float> const&, sead::Color4f const&);
        void static drawLineImm(agl::DrawContext*, sead::Vector3<float> const&, sead::Vector3<float> const&, sead::Color4f const&, float);
        void static drawFrameBuffer(agl::DrawContext *,agl::RenderBuffer const&,sead::Viewport const&,agl::utl::ImageFilter2D::Channel);
    };
};