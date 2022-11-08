#pragma once

namespace sead {
    class LookAtCamera;
    class Projection;
    class GraphicsContextMRT;
}

namespace al {
    class IUseCamera;
    class Scene;

    void updateKitListPrev(Scene *);
    void updateKitList(Scene *, const char *);
    void updateKitListPost(Scene *);

    void setContextMRT(sead::GraphicsContextMRT *);
    void setContextMRTBlendBcLbuf(sead::GraphicsContextMRT *,bool);
    void setContextMRTAddBcLbuf(sead::GraphicsContextMRT *,bool);
    void setContextMRTBlendBcNrmLbuf(sead::GraphicsContextMRT *,bool);
    void setContextMRTBlendAll(sead::GraphicsContextMRT *,bool);
    void setContextMRTBlendLbuf(sead::GraphicsContextMRT *,bool);
    void setContextMRTAddLbuf(sead::GraphicsContextMRT *,bool);
    void setContextMRTMulAddLbuf(sead::GraphicsContextMRT *,bool);
    void setContextMRTMulLbuf(sead::GraphicsContextMRT *,bool);
    void setContextMRTMulBc(sead::GraphicsContextMRT *,bool);
    void setContextMRTFootPrint(sead::GraphicsContextMRT *);
    void setContextMRTAlphaMask(sead::GraphicsContextMRT *);
    void setContextMRTMiiFaceXlu(sead::GraphicsContextMRT *);
    void setContextMRTOnlyDepth(sead::GraphicsContextMRT *);
    void setContextMRTSilhouette(sead::GraphicsContextMRT *);

    sead::LookAtCamera *getLookAtCamera(al::IUseCamera const*,int);
} // namespace al



// TODO: get this out of here
namespace rs
{
    void requestGraphicsPresetAndCubeMapPause(const al::Scene *);
} // namespace rs
