#pragma once

#include "GameSystemInfo.h"
#include "al/nerve/NerveExecutor.h"
#include "al/hio/HioNode.h"

class GameSystem : public al::NerveExecutor, public al::HioNode {
    public:
        GameSystem();
        ~GameSystem();
        void init();
        void setPadName();
        void tryChangeSequence(char const*);
        void movement();
        void drawMain();
        void exePlay();
        al::GameSystemInfo* mSystemInfo;  // 0x18
        // 0x78 GameConfigData
};