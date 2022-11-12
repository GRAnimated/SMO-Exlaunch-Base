#pragma once

#include "al/effect/EffectKeeper.h"

namespace al {
    class ActorResource;
    class ActionAnimCtrlInfo {
        public:
        ActionAnimCtrlInfo(int);
    };
    class ActionAnimCtrl {
        public:
        ActionAnimCtrl *tryCreate(al::LiveActor *,al::ActorResource const*,char const*,char const*);
        void init(al::ActorResource const*,char const*,char const*);
        void start(char const*);
        ActionAnimCtrlInfo *findAnimInfo(char const*);
        void getFrame(void);
        void getActionFrameMax(char const*);
        void getFrameRate(void);
        void trySetFrame(float);
        void isExistAction(char const*);
        void isActionOneTime(char const*);
        void isActionEnd(void);
        void getPlayingActionName(void);
        ActionAnimCtrl(al::LiveActor *);
        void sortCtrlInfo(void);
    };
}