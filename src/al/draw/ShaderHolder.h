#pragma once

#include "agl/utl.h"
#include "heap/seadDisposer.h"

namespace al {
    class ShaderHolder {
        SEAD_SINGLETON_DISPOSER(ShaderHolder);
        public:
        ShaderHolder();
        virtual ~ShaderHolder();

        void init();
        void initAndLoadAllFromDir(char const*,sead::Heap *,sead::Heap *);
        void loadAll(char const*,sead::Heap *,char const*);
        void load(char const*,char const*,sead::Heap *,uint);
        bool tryGetShaderProgram(char const*) const;
        agl::ShaderProgram const &getShaderProgram(char const*) const;
        //void getShadingModel(char const*,char const*) const;
    };
}