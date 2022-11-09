#pragma once

#include "al/actor/ActorActionKeeper.h"
#include "al/hio/HioNode.h"


namespace al {
    class ModelCtrl;

    class ModelKeeper : public al::IUseHioNode {
    public:
        const char *mResourceName;
        al::ModelCtrl *mModelCtrl;
        const al::ActorResource *mResource;
        _BYTE gap20[67];
        _BYTE field_63;
    };
}
