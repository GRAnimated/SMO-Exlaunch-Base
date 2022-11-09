#pragma once

#include "al/draw/GraphicsSystemInfo.h"
#include "types.h"
#include "al/hio/HioNode.h"
#include "sead/prim/seadSafeString.h"

namespace sead {
    class LookAtCamera;
    class Projection;
    class GraphicsContextMRT;
}

namespace al {
    class IUseCamera;
    class Scene;
    class Resource;
    class ActorResource : public al::IUseHioNode {
        public:
        sead::FixedSafeString<128> field_8;
        al::Resource *mResourceModel;
        al::Resource *mResourceAnim;
        _BYTE byteB0;
        int qwordB8;
        int qwordC0;

    };

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

    al::Resource *getResourceName(al::Resource const*);
    al::Resource *getResourcePath(al::Resource const*);
} // namespace al



// TODO: get this out of here
namespace rs
{
    void requestGraphicsPresetAndCubeMapPause(const al::Scene *);
} // namespace rs
