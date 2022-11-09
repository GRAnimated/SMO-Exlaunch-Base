//#pragma once
//
//#include "CameraPoseUpdater.h"
//
//namespace al
//{
//    struct CameraInputHolder {
//        class ICameraInput* cameraInput;
//    };
//
//    class CameraDirector {
//        public:
//            void startSnapShotMode(bool);
//            al::CameraPoseUpdater *getPoseUpdater(int) const;
//            float getSceneFovyDegree(void) const;
//
//            unsigned char padding[0x30-0x02];
//            float sceneFovyDegree; // 0x30
//            void* padding2[4];
//            CameraInputHolder* mCameraInputHolder;
//            // 0xBC float farClipDistance
//            // 0xB8 float nearClipDistance
//    };
//
//    class IUseCamera
//    {
//    public:
//        virtual al::CameraDirector* getCameraDirector() const = 0;
//    };
//};

#pragma once

#include "CameraPoseUpdater.h"
#include "al/camera/CameraPoser.h"
#include "al/camera/CameraTicket.h"
#include "al/hio/HioNode.h"

namespace al {

    class CameraResourceHolder;
    class CameraPoserFactory;
    class CameraRailHolder;
    class PlayerHolder;
    class ICameraInput;
    class CameraPoserSceneInfo;
    class NameToCameraParamTransferFunc;
    class IUseExecutor {};

    class CameraDirector : public al::HioNode, public al::IUseExecutor {
        public:
            
            void init(al::CameraPoserSceneInfo *,al::CameraPoserFactory const*);
            al::CameraPoseUpdater *getPoseUpdater(int) const;
            void endInit(al::PlayerHolder const*);
            al::CameraTicket *createCameraFromFactory(char const *creatorName, al::PlacementId const*,char const*, int priority, sead::Matrix34f const&);
            void createCamera(al::CameraPoser *,al::PlacementId const*,char const*,int,sead::Matrix34f const&,bool);
            void execute(void);
            void update(void);
            void createObjectCamera(al::PlacementId const*,char const*,char const*,int,sead::Matrix34f const&);
            void createObjectEntranceCamera(al::PlacementId const*,char const*,sead::Matrix34f const&);
            void createMirrorObjectCamera(al::PlacementId const*,char const*,int,sead::Matrix34f const&);
            void initAreaCameraSwitcherMultiForPrototype(al::AreaObjDirector *);
            al::ICameraInput *getCameraInput(int);
            void setCameraInput(al::ICameraInput const*);
            void setViewCameraInput(al::ICameraInput const*,int);
            void initAreaCameraSwitcherSingle(void);
            void initResourceHolder(al::CameraResourceHolder const*);
            void registerCameraRailHolder(al::CameraRailHolder *);
            void initSceneFovyDegree(float);
            void setCameraParamTransferFuncTable(al::NameToCameraParamTransferFunc const*,int);
            void initSettingCloudSea(float);
            void initSnapShotCameraAudioKeeper(al::IUseAudioKeeper *);
            void initAndCreatePauseCameraCtrl(float);
            float getSceneFovyDegree(void) const;
            void validateCameraArea2D(void);
            void invalidateCameraArea2D(void);
            void stopByDeathPlayer(void);
            void restartByDeathPlayer(void);
            void startInvalidStopJudgeByDemo(void);
            void endInvalidStopJudgeByDemo(void);
            void startSnapShotMode(bool);
            void endSnapShotMode(void);

            unsigned char padding[0x20];
            al::CameraPoserFactory *mFactory; // 0x28
            al::CameraPoserSceneInfo *mSceneInfo; // 0x30
            // 0xBC float farClipDistance
            // 0xB8 float nearClipDistance
    };

    class IUseCamera
    {
    public:
        virtual al::CameraDirector* getCameraDirector() const = 0;
    };
};