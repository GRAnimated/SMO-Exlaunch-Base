#include "agl/DrawContext.h"
#include "al/util/GraphicsUtil.h"
#include "gfx/seadFrameBuffer.h"
#include "hook/replace_hook.hpp"
#include "hook/trampoline_hook.hpp"
//#include "nx/kernel/svc.h"
#include "nx/kernel/svc.h"
#include "quick/quick.hpp"
#include "lib.hpp"
#include "patches.hpp"
#include "nn/err.h"
#include "logger/Logger.hpp"
#include "fs.h"

#include <basis/seadRawPrint.h>
#include <prim/seadSafeString.h>
#include <resource/seadResourceMgr.h>
#include <filedevice/nin/seadNinSDFileDeviceNin.h>
#include <filedevice/seadFileDeviceMgr.h>
#include <filedevice/seadPath.h>
#include <resource/seadArchiveRes.h>
#include <framework/seadFramework.h>
#include <heap/seadHeapMgr.h>
#include <heap/seadExpHeap.h>
#include <devenv/seadDebugFontMgrNvn.h>
#include <gfx/seadTextWriter.h>
#include "agl/utl.h"
#include "al/draw/GBufferArray.h"
#include "al/draw/ShaderHolder.h"
#include "agl/RenderBuffer.h"

#include "game/StageScene/StageScene.h"
#include "game/System/GameSystem.h"
#include "game/HakoniwaSequence/HakoniwaSequence.h"
#include "al/draw/GraphicsSystemInfo.h"
#include "rs/util.hpp"

#include "al/util.hpp"
#include "al/fs/FileLoader.h"

#include "program/internals/DebugDrawing.h"

#include "sead/gfx/seadPrimitiveRenderer.h"
#include "sead/gfx/seadGraphicsContext.h"


static const char *DBG_FONT_PATH   = "DebugData/Font/nvn_font_jis1.ntx";
static const char *DBG_SHADER_PATH = "DebugData/Font/nvn_font_shader_jis1.bin";
static const char *DBG_TBL_PATH    = "DebugData/Font/nvn_font_jis1_tbl.bin";

sead::TextWriter *gTextWriter;

float speedGain = 0.0f;
void graNoclipCode(al::LiveActor *player) {

    static bool isFirst = true;

    float speed = 25.0f;
    float speedMax = 150.0f;
    float vspeed = 20.0f;

    sead::Vector3f *playerPos = al::getTransPtr(player);
    sead::Vector3f *cameraPos = al::getCameraPos(player, 0);
    sead::Vector2f *leftStick = al::getLeftStick(-1);
    
    // Its better to do this here because loading zones reset this.
    al::offCollide(player);
    al::setVelocityZero(player);
    
    // Mario slightly goes down even when velocity is 0. This is a hacky fix for that.
    playerPos->y += 1.4553f;
    
    float d = sqrt(al::powerIn(playerPos->x - cameraPos->x, 2) + (al::powerIn(playerPos->z - cameraPos->z, 2)));
    float vx = ((speed + speedGain)/d)*(playerPos->x - cameraPos->x);
    float vz = ((speed + speedGain)/d)*(playerPos->z - cameraPos->z);
    
    if (!al::isPadHoldZR(-1)) {
        playerPos->x -= leftStick->x * vz;
        playerPos->z += leftStick->x * vx;
        
        playerPos->x += leftStick->y * vx;
        playerPos->z += leftStick->y * vz;
        
        if (al::isPadHoldX(-1)) speedGain -= 0.5f;
        if (al::isPadHoldY(-1)) speedGain += 0.5f;
        if (speedGain <= 0.0f) speedGain = 0.0f;
        if (speedGain >= speedMax) speedGain = speedMax;
        
        if (al::isPadHoldZL(-1) || al::isPadHoldA(-1)) playerPos->y -= (vspeed + speedGain/6);
        if (al::isPadHoldB(-1)) playerPos->y += (vspeed + speedGain/6);
    }
}

static bool lockLODs = false;
static bool isNoclip = false;

void controlLol(StageScene* scene) {
    auto actor = rs::getPlayerActor(scene);

    
    

    if(al::isPadTriggerRight(-1)) {
        isNoclip = !isNoclip;

        if(!isNoclip) {
            al::onCollide(actor);
        }
    }

    if (al::isPadTriggerUp(-1)) lockLODs = !lockLODs;

    if(isNoclip) {
        graNoclipCode(actor);
    }
}

//HOOK_DEFINE_TRAMPOLINE(LockLODHook) {
//    static void Callback(void* modelCtrl) {
//        if (lockLODs) return;
//        Orig(modelCtrl);   
//    }
//};

Quick_TOGGLE(ModelCtrlUpdateLock, lockLODs)



//Quick_TOGGLE(ModelDrawerForwardDraw, lockLODs)

HOOK_DEFINE_TRAMPOLINE(ControlHook) {
    static void Callback(StageScene* scene) {
        controlLol(scene);
        Orig(scene);   
    }
};

HOOK_DEFINE_TRAMPOLINE(DisableUserExceptionHandler) {
    static void Callback(void (*)(nn::os::UserExceptionInfo*), void*, u64, nn::os::UserExceptionInfo*) {
        Logger::log("this is so cool\n");

        static char ALIGNED(0x1000) exceptionStack[0x6000];
        static nn::os::UserExceptionInfo exceptionInfo;
        Orig([](nn::os::UserExceptionInfo* exceptionInfo){
            Logger::log("Among us!!!!!! %p\n", exceptionInfo->PC.x);
            for (size_t i = 0; i < 29; i++)
            {
                Logger::log("my nuts! %p\n", exceptionInfo->CpuRegisters[i].x);
            }
        }, exceptionStack, sizeof(exceptionStack), &exceptionInfo);

    }
};

HOOK_DEFINE_REPLACE(ReplaceSeadPrint) {
    static void Callback(const char* format, ...) {
        va_list args;
        va_start(args, format);
        Logger::log(format, args);
        va_end(args);
    }
};

HOOK_DEFINE_TRAMPOLINE(CreateFileDeviceMgr) {
    static void Callback(sead::FileDeviceMgr *thisPtr) {
        
        Orig(thisPtr);

        thisPtr->mMountedSd = nn::fs::MountSdCardForDebug("sd").isSuccess();

        sead::NinSDFileDevice *sdFileDevice = new sead::NinSDFileDevice();

        thisPtr->mount(sdFileDevice);
    }
};

HOOK_DEFINE_TRAMPOLINE(RedirectFileDevice) {
    static sead::FileDevice *Callback(sead::FileDeviceMgr *thisPtr, sead::SafeString &path, sead::BufferedSafeString *pathNoDrive) {

        sead::FixedSafeString<32> driveName;
        sead::FileDevice* device;

        Logger::log("Path: %s\n", path.cstr());

        if (!sead::Path::getDriveName(&driveName, path))
        {
            
            device = thisPtr->findDevice("sd");

            if(!(device && device->isExistFile(path))) {

                device = thisPtr->getDefaultFileDevice();

                if (!device)
                {
                    Logger::log("drive name not found and default file device is null\n");
                    return nullptr;
                }

            }else {
                Logger::log("Found Replacement File on SD! Path: %s\n", path.cstr());
            }
            
        }
        else
            device = thisPtr->findDevice(driveName);

        if (!device)
            return nullptr;

        if (pathNoDrive != NULL)
            sead::Path::getPathExceptDrive(pathNoDrive, path);

        return device;
    }
};

HOOK_DEFINE_TRAMPOLINE(FileLoaderLoadArc) {
    static sead::ArchiveRes *Callback(al::FileLoader *thisPtr, sead::SafeString &path, const char *ext, sead::FileDevice *device) {

        Logger::log("Path: %s\n", path.cstr());

        sead::FileDevice* sdFileDevice = sead::FileDeviceMgr::instance()->findDevice("sd");

        if(sdFileDevice && sdFileDevice->isExistFile(path)) {

            Logger::log("Found Replacement File on SD! Path: %s\n", path.cstr());

            device = sdFileDevice;
        }

        return Orig(thisPtr, path, ext, device);
    }
};

void searchHook(agl::SamplerLocation* _this, agl::ShaderProgram &program) {
    _this->shaderName = "cWorldNormal";
    _this->search(program);
}

void newersearchHook(agl::SamplerLocation* _this, agl::ShaderProgram &program) {
    _this->shaderName = "uDepth";
    _this->search(program);
}

//void newSearchHook(agl::TextureSampler *_this) {
//    void *aaa;
//    __asm("MOV %0, X23" : "=r" (aaa));
//    //agl::TextureSampler::applyTextureData(aaa->)
//}

agl::TextureData *GBufBaseColorTex;
al::GBufferArray *GBufferArray;

HOOK_DEFINE_REPLACE(getGBufBaseColorTex) {
    static agl::TextureData *Callback(al::GBufferArray *buf) {
        GBufBaseColorTex = buf->mGBufBaseColor;
        GBufferArray = buf;
        return buf->mGBufBaseColor;
    }
};

static bool renderthat = false;

void HookHakoniwaSequenceDrawMain(HakoniwaSequence *curSequence, sead::Viewport *viewport, sead::DrawContext *ctx) {
    //if (al::isPadTriggerL(-1)) renderthat = !renderthat;

    if (curSequence->curScene && renderthat == true) {
        sead::LookAtCamera *cam = al::getLookAtCamera(curSequence->curScene, 0);
        sead::Projection *projection = al::getProjectionSead(curSequence->curScene, 0);

        auto prim = sead::PrimitiveRenderer::instance();

        sead::GraphicsContext context;
        //context.applyAlphaTest(ctx);
        //context.applyDepthAndStencilTest(ctx);
        //context.applyColorMask(ctx);
        //context.applyBlendAndFastZ(ctx);
        //context.applyBlendConstantColor(ctx);
        //context.applyCullingAndPolygonModeAndPolygonOffset(ctx);
        //al::setContextMRT(&context);
        //al::setContextMRTBlendBcLbuf(&context, 1);
        //al::setContextMRTAddBcLbuf(&context, 1);
        //al::setContextMRTBlendBcNrmLbuf(&context, 0);
        //al::setContextMRTBlendAll(&context, 1);
        //al::setContextMRTBlendLbuf(&context, 1);
        //al::setContextMRTAddLbuf(&context, 1);
        //al::setContextMRTMulAddLbuf(&context, 1);
        //al::setContextMRTMulLbuf(&context, 1);
        //al::setContextMRTMulBc(&context, 1);
        //al::setContextMRTFootPrint(&context);
        //al::setContextMRTAlphaMask(&context);
        //al::setContextMRTMiiFaceXlu(&context);
        //al::setContextMRTOnlyDepth(&context);
        //al::setContextMRTSilhouette(&context);
        //context.apply(ctx);


        //context.mCullingMode = sead::Graphics::cCullingMode_Front;
        context.mDepthFunc = sead::Graphics::cDepthFunc_Never;
        context.mBlendEquationRGB = sead::Graphics::cBlendEquation_Add;
        context.mBlendEquationA = sead::Graphics::cBlendEquation_Add;
        context.mBlendFactorSrcA = sead::Graphics::cBlendFactor_One;
        context.mBlendFactorDstA = sead::Graphics::cBlendFactor_Zero;
        context.mBlendFactorSrcRGB = sead::Graphics::cBlendFactor_SrcAlpha;
        context.mBlendFactorDstRGB = sead::Graphics::cBlendFactor_InvSrcAlpha;
        //al::setContextMRTBlendBcNrmLbuf(&context, 0);
        context.mBlendFunc = sead::Graphics::cDepthFunc_LessEqual;
        context.mDepthTestEnable = true;
        context.mDepthWriteEnable = true;

        context.apply(ctx);


        prim->mDrawer.setDrawContext(ctx);
        prim->setCamera(*cam);
        prim->setProjection(*projection);
        prim->setModelMatrix(sead::Matrix34f::ident);

        prim->begin();
        prim->drawCube(sead::PrimitiveDrawer::CubeArg(sead::Vector3f::zero, sead::Vector3f(1000.f, 2000.0f, 1000.0f), sead::Color4f(0.f, 0.f, 1.f, .5f)));
        prim->end();

        //auto prim = sead::PrimitiveRenderer::instance();
        //prim->mDrawer.setDrawContext(drawContext);
        //prim->setCamera(*cam);
        //prim->setProjection(*projection);
        //prim->setModelMatrix(sead::Matrix34f::ident);


    }
    
    //if (al::isPadHoldLeft(-1) && GBufBaseColorTex != nullptr) {
    //    //auto ShaderHolder = al::ShaderHolder::instance();
//
    //    //agl::ShaderProgram prgm = ShaderHolder->getShaderProgram("alRenderMaterial");
    //
    //    //auto loc = agl::SamplerLocation("uDepth", -1);
    //    //loc.search(prgm);
//
    //    sead::GraphicsContext context;
    //    context.apply(drawContext);
//
    //    const agl::SamplerLocation loc = al::getSamplerLocationGBufferBaseColor();
    //    svcOutputDebugString("got loc", 0x100);
//
    //    auto sampler = agl::TextureSampler();
    //    svcOutputDebugString("got sampler", 0x100);
    //    sampler.applyTextureData(*GBufBaseColorTex);
    //    svcOutputDebugString("applied texture data to sampler", 0x100);
    //    sampler.activate((agl::DrawContext *)drawContext, loc, 1, 0);
    //    svcOutputDebugString("activated sampler", 0x100);
//
    //    agl::utl::ImageFilter2D::drawTextureQuadTriangle((agl::DrawContext *)drawContext, sampler);
    //    svcOutputDebugString("drew quad", 0x100);
    //}
    //if (al::isPadTriggerL(-1)) svcOutputDebugString(GBufBaseColorTex != nullptr ? "true" : "false", 0x100);

    al::executeDraw(curSequence->mLytKit, "２Ｄバック（メイン画面）");

    __asm("LDR X0, [X19,#0xF8]");
}



void HookRender(agl::RenderBuffer *buffer, sead::DrawContext *ctx) {
    agl::TextureData *fuck;
    __asm("MOV %0, X24" : "=r" (fuck));
    if (al::isPadHoldLeft(-1) && fuck != nullptr) {
        //auto ShaderHolder = al::ShaderHolder::instance();

        //agl::ShaderProgram prgm = ShaderHolder->getShaderProgram("alRenderMaterial");
    
        //auto loc = agl::SamplerLocation("uDepth", -1);
        //loc.search(prgm);

        //sead::GraphicsContext context;
        //context.apply(ctx);
//
        //const agl::SamplerLocation loc = al::getSamplerLocationGBufferBaseColor();
        //svcOutputDebugString("got loc", 0x100);
//
        //auto sampler = agl::TextureSampler();
        //svcOutputDebugString("got sampler", 0x100);
        //sampler.applyTextureData(*fuck);
        //svcOutputDebugString("applied texture data to sampler", 0x100);
        //sampler.activate((agl::DrawContext *)ctx, loc, 1, 0);
        //svcOutputDebugString("activated sampler", 0x100);
//
        //agl::utl::ImageFilter2D::drawTextureQuadTriangle((agl::DrawContext *)ctx, sampler);
        //svcOutputDebugString("drew quad", 0x100);
    }

    if (al::isPadTriggerL(-1)) svcOutputDebugString(fuck != nullptr ? "true" : "false", 0x100);

    ((sead::FrameBuffer *)buffer)->bind(ctx);
}


static int channel = 0;
static int texRenderSize = 0;
static int bufo = 0;
static bool renderIt = false;

#include <sead/prim/seadStringUtil.h>

void ViewportApplyHook(sead::Viewport *viewport, agl::DrawContext *ctx, agl::RenderBuffer *buffer) {
    if (al::isPadHoldZL(-1)) {
        if (al::isPadTriggerL(-1)) texRenderSize -= 1;
        if (al::isPadTriggerR(-1)) texRenderSize += 1;
    } else if (al::isPadHoldZR(-1)) {
        if (al::isPadTriggerL(-1)) bufo -= 1;
        if (al::isPadTriggerR(-1)) bufo += 1;
        if (al::isPadTriggerLeft(-1)) renderIt = !renderIt;
    } else {
        if (al::isPadTriggerL(-1)) channel -= 1;
        if (al::isPadTriggerR(-1)) channel += 1;
    }
    
    //if (al::isPadHoldLeft(-1)) agl::utl::DevTools::drawFrameBuffer((agl::DrawContext *)ctx, (agl::RenderBuffer &)buffer, *viewport, agl::utl::ImageFilter2D::Channel(channel));
    if (GBufferArray && renderIt == true) {
        agl::TextureData *apply = GBufferArray->mGBufNrmWorld;
        switch (bufo) {
            case 0:
                apply = GBufferArray->mGBufLightBuffer;
                break;
            case 1:
                apply = GBufferArray->mGBufNrmWorld;
                break;
            case 2:
                apply = GBufferArray->mGBufDepthView;
                break;
            case 3:
                apply = GBufferArray->mGBufBaseColor;
                break;
            case 4:
                apply = GBufferArray->mGBufMotionVec;
                break;
        }

        char buffer[256];
        int bufeoo = sead::StringUtil::snprintf(buffer, 256, "%i, %i, %i", apply->mSurface._0, apply->mSurface._2, apply->mSurface._4);
        if (al::isPadTriggerLeft(-1)) svcOutputDebugString(buffer, 256);

        //svcOutputDebugString("about to draw", 0x50);
        sead::GraphicsContext context;
        context.apply(ctx);
        auto sampler = agl::TextureSampler();
        sampler.applyTextureData(*apply);
        agl::utl::ImageFilter2D::drawColorQuadTriangle(ctx, sead::Color4f::cBlack, 1.0f);
        agl::utl::ImageFilter2D::drawTextureChannel(ctx, sampler, *viewport, channel, sead::Vector2f(1280.f/apply->mSurface._0, 720.f/apply->mSurface._2), sead::Vector2f::zero);

    }

    sead::Camera *cam;
    __asm("MOV %0, X28" : "=r" (cam));

    sead::Projection *projection;
    __asm("MOV %0, X26" : "=r" (projection));

    if (renderthat) {
        
    }

    //svcOutputDebugString(buffer->_10->_120, 0x100);
    //svcOutputDebugString(buffer->_18->_120, 0x100);
    //svcOutputDebugString(buffer->_20->_120, 0x100);
    //svcOutputDebugString(buffer->_28->_120, 0x100);

    //if (buffer->_8) svcOutputDebugString("_8 true", 0x50);
    //if (buffer->_10) svcOutputDebugString("_10 true", 0x50);
    //if (buffer->_18) svcOutputDebugString("_18 true", 0x50);
    //if (buffer->_20) svcOutputDebugString("_20 true", 0x50);
    //if (buffer->_28) svcOutputDebugString("_28 true", 0x50);
    //if (buffer->_30) svcOutputDebugString("_30 true", 0x50);
    //if (buffer->_38) svcOutputDebugString("_38 true", 0x50);
    //if (buffer->_40) svcOutputDebugString("_40 true", 0x50);
    //if (buffer->_48) svcOutputDebugString("_48 true", 0x50);
    //if (buffer->_50) svcOutputDebugString("_50 true", 0x50);
    //if (buffer->_58) svcOutputDebugString("_58 true", 0x50);
    //if (buffer->_60) svcOutputDebugString("_60 true", 0x50);


    //agl::utl::ImageFilter2D::drawTexture3D((agl::DrawContext *)ctx, sampler, *viewport, 0.0f, sead::Vector2f::zero, sead::Vector2f::zero, 0.0f);
    viewport->apply(ctx, (sead::LogicalFrameBuffer &)buffer);
}



void hookBlaBlaBlaBlaBla() {
    
    exl::patch::CodePatcher p(0x0050F258);
    p.Write(0xAA1303E0); /* MOV X0, X19*/

    p.Seek(0x0050F25C);
    p.Write(0x910003E1); /* MOV X1, SP*/

    p.Seek(0x0050F264);
    p.Write(0xAA1403E2); /* MOV X2, X20*/

    p.Seek(0x0050F268);
    p.BranchLinkInst((void*) HookHakoniwaSequenceDrawMain);
}

// 00942D8C - important - NOPing disables almost all meshes from drawing


extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking enviroment. */
    envSetOwnProcessHandle(exl::util::proc_handle::Get());
    exl::hook::Initialize();

    runCodePatches();

    hookBlaBlaBlaBlaBla();

    //exl::patch::CodePatcher baseColPatch(0x009FEB8C);
    //baseColPatch.BranchInst((void*) copyBaseColorTex);

    getGBufBaseColorTex::InstallAtOffset(0x009FEB88);
    

    //exl::patch::CodePatcher render(0x0087FF64);
    //render.BranchLinkInst((void*) HookRender);

    exl::patch::CodePatcher render(0x0087FF74);
    render.BranchLinkInst((void*) ViewportApplyHook);
    

    //exl::patch::CodePatcher disableDraws(0x00942D8C);
    //disableDraws.WriteInst(exl::patch::inst::Nop());

    //Quick_TOGGLE_install(ModelDrawerForwardDraw, "_ZNK2al19ModelDrawerDeferred4drawEv")

    //exl::patch::CodePatcher searchPatch(0x009BBFD8);
    //searchPatch.BranchLinkInst((void*) searchHook);
//
    //searchPatch.Seek(0x009B4178);
    //searchPatch.Write(0xAA1903E3); /* MOV X3, X25*/

    //searchPatch.Seek(0x009BBFEC);
    //searchPatch.WriteInst(exl::patch::inst::Nop());

    //searchPatch.Seek(0x009BC068);
    //009BC174
    //searchPatch.BranchInst(0x009BC174);

    //exl::patch::CodePatcher newSearchPatch(0x0087FE04);
    //newSearchPatch.BranchLinkInst((void*) newSearchHook);

    //exl::patch::CodePatcher newersearchPatch(0x0087FDEC);
    //newersearchPatch.BranchLinkInst((void*) newersearchHook);


    // R_ABORT_UNLESS(Logger::instance().init("64.201.219.20", 3080).value);
    //R_ABORT_UNLESS(Logger::instance().init("10.0.0.224", 3080).value);

    // SD File Redirection

    RedirectFileDevice::InstallAtOffset(0x76CFE0);
    FileLoaderLoadArc::InstallAtOffset(0xA5EF64);
    CreateFileDeviceMgr::InstallAtOffset(0x76C8D4);


    // Sead Debugging Overriding

    ReplaceSeadPrint::InstallAtOffset(0xB59E28);
    ControlHook::InstallAtSymbol("_ZN10StageScene7controlEv");

    Quick_TOGGLE_install(ModelCtrlUpdateLock, "_ZN2al9ModelCtrl8calcViewEv")
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}