#pragma once

#include "al/scene/Scene.h"
#include "game/Player/YukimaruRacePlayer.h"
#include "game/StageScene/StageSceneLayout.h"
#include "sead/prim/seadSafeString.h"
#include "al/LiveActor/LiveActorKit.h"
#include "types.h"

//#define INHERITSIZE sizeof(al::Scene)

/*
class StageScene : public al::Scene
{
    public:

        bool isPause() const;
        // 0x88 StageResourceKeeper *
        // 0x90 LiveActorKit *
        // 0x98 LayoutKit *
        // 0xA0 SceneObjHolder *
        // 0xA8 SceneStopCtrl *

        unsigned char padding_2D0[0x2D0 - INHERITSIZE];
        GameDataHolderAccessor *mHolder; // 0x2D0
        unsigned char padding_2F8[0x20];
        StageSceneLayout *stageSceneLayout; // 0x2F8
};
*/

namespace al {
    class Projection;
    class WipeHolder;
	class WipeSimple {
        public:
        void startOpen(int);
        void startClose(int);
    };
	class WindowConfirm;
	class HtmlViewer;
    class SimpleLayoutAppearWaitEnd;
    class SimpleAudioUser;
    class CameraTicket;
    class ParabolicPath;
    class DemoSyncedEventKeeper;
    class ChromakeyDrawer;
    class NfpDirector;
	class LayoutTextureRenderer;
}

class StageSceneStateTimeBalloon;
class StageSceneStateWorldMap;
class StageSceneStateShop;
class StageSceneStateSnapShot;
class StageSceneStateGetShine;
class StageSceneStateGetShineMain;
class StageSceneStateGetShineGrand;
class StageSceneStateCollectBgm;
class StageSceneStateStageMap;
class StageSceneStateMiniGameRanking;
class StageSceneStatePauseMenu;
class StageSceneStateCloset;
class StageSceneStateSkipDemo;
class StageSceneStateCheckpointWarp;
class StageSceneStateCarryMeat;
class TimeBalloonSequenceInfo;
class StageSceneStateTitleLogo;
class StageSceneStateMiss;
//class StageSceneStateYukimaruRace;
class StageSceneStateYukimaruRaceTutorial;
class StageSceneStateRaceManRace;
class StageSceneStateRadicon;
class StageSceneStateScenarioCamera;
class StageSceneStateGetShineMainSandWorld;
class StageSceneStateRecoverLife;
class StageSceneStateTalk;
class StageSceneStateWarp;
class StageSceneStateWorldIntroCamera;
class ScenarioStartCameraHolder;
class CinemaCaption;
class MapLayout;
class CollectionList;
class TimeBalloonNpc;
class ProjectItemDirector;
class Pyramid;
class PlayGuideSkip;
class LocationNameCtrl;
class MiniGameMenu;
class NpcEventDirector;
class ControllerGuideSnapShotCtrl;
class InputSeparator;
class OpeningStageStartDemo;
class SceneAudioSystemPauseController;
class DemoSoundSynchronizer;
class ProjectSeNamedList;
class TimeBalloonDirector;
class KoopaHackFunction;
class CollectBgmRegister;
class BgmAnimeSyncDirector;
class HelpAmiiboDirector;

class StageSceneStateRaceIntroCamera;
class StageSceneStateRaceCountDown;
class StageSceneStateRacePause;
class RaceLayout;
class RaceWatcher;

class StageSceneStateYukimaruRace : public al::NerveStateBase {
    public:
    al::Scene *mStageScene;
    al::SceneObjHolder *sceneObjHolder;
    StageSceneStateRaceIntroCamera *mStateRaceIntroCamera;
    StageSceneStateRaceCountDown *mStateRaceCountDown;
    StageSceneStateRacePause *mStateRacePause;
    StageSceneStateSnapShot *mStateSnapShot;
    RaceLayout *mRaceLayout;
    YukimaruRacePlayer *mPlayer;
    RaceWatcher *mRaceWatcher;
    int winPlace;
};

class StageScene : public al::Scene
{
    public:
    // al::Scene has a size of D8
    
    sead::SafeStringBase<char> *currentStageName; // D8
    char *currentStageNamePtr; // E0
    int *worldId; // E8
    _BYTE gap_E8[0x40]; // F0 - 130
    u32 *scenarioNo; // 130
    
    // States - 0x138 to 0x208
    StageSceneStateWorldMap *mStateWorldMap; // 138
    StageSceneStateShop *mStateShop; // 140
    StageSceneStateSnapShot *mStateSnapShot; // 148
    StageSceneStateGetShine *mStateGetShine; // 150
    StageSceneStateGetShineMain *mStateGetShineMain; // 158
    StageSceneStateGetShineGrand *mStateGetShineGrand; // 160
    StageSceneStateCollectBgm *mStateCollectBgm; // 168
    StageSceneStateStageMap *mStateStageMap; // 170
    StageSceneStateMiniGameRanking *mStateMiniGameRanking; // 178
    StageSceneStatePauseMenu *mStatePauseMenu; // 180
    StageSceneStateCloset *mStateCloset; // 188
    StageSceneStateSkipDemo *mStateSkipDemo; //190
    StageSceneStateCheckpointWarp *mStateCheckpointWarp; // 198
    StageSceneStateCarryMeat *mStateCarryMeat; // 1A0
    StageSceneStateTimeBalloon *mStateTimeBalloon; // 1A8
    StageSceneStateTitleLogo *mStateTitleLogo; // 1B0
    StageSceneStateMiss *mStateMiss; // 1B8
    StageSceneStateYukimaruRace *mStateYukimaruRace; // 1C0
    StageSceneStateYukimaruRaceTutorial *mStateYukimaruRaceTutorial; // 1C8
    StageSceneStateRaceManRace *mStateRaceManRace; // 1D0
    StageSceneStateRadicon *mStateRadicon; // 1D8
    StageSceneStateScenarioCamera *mStateScenarioCamera; // 1E0
    StageSceneStateRecoverLife *mStateRecoverLife; // 1E8
    StageSceneStateGetShineMainSandWorld *mStateGetShineMainSandWorld; // 1F0
    StageSceneStateTalk *mStateTalk; // 1F8
    StageSceneStateWarp *mStateWarp; // 200
    StageSceneStateWorldIntroCamera *mStateWorldIntroCamera; // 208
    
    ScenarioStartCameraHolder *mScenarioStartCameraHolder; // 210
    sead::SafeStringBase<char> *costumeName; // 218
	sead::SafeStringBase<char> *newCostumeName; // 220
    _BYTE gap_228[0x48]; // 228 to 270
    sead::SafeStringBase<char> *capName; // 270
	sead::SafeStringBase<char> *newCapName; // 278
    _BYTE gap_280[0x50]; // 280 to 2D0
    GameDataHolder *mDataHolderBase; // 2D0
	al::PlacementInfo *mPlacementInfo;// 2D8
	al::LayoutTextureRenderer *mLayoutTextureRenderer; //2E0
	PlayGuideSkip *mPlayGuideSkip; // 2E8
    CinemaCaption *mCinemaCaption; // 2F0
    StageSceneLayout *mStageSceneLayout; // 2F8
    bool enableLayout; // 300
	_BYTE gap_301[0x7]; // 301-308
	al::SimpleLayoutAppearWaitEnd *mLayoutStartScenario; // 308
	al::SimpleLayoutAppearWaitEnd *mLayoutStartWorld; // 310
	al::SimpleLayoutAppearWaitEnd *mLayoutStartWorldRegion; // 318
	ControllerGuideSnapShotCtrl *mControllerGuideSnapShotCtrl; // 320
	InputSeparator *mInputSeparator; // 328
	al::WipeSimple *mWipeCircle; // 330
    al::WipeHolder *mWipeHolder; // 338
    LocationNameCtrl *mLocationNameCtrl; // 340
    al::WindowConfirm *mWindowConfirm; // 348
    MiniGameMenu *mMiniGameMenu; // 350
    bool *hasStartedStage; // 358 // probably wrong
    _BYTE gap_360[0x10]; // 360
    MapLayout *mMapLayout; // 370
    al::SimpleLayoutAppearWaitEnd *mMenuLayout; // 378
    al::LiveActorGroup *mShoppingWatcher; // 380
    CollectionList *mCollectionList; // 388
    __int64_t unkNoIdea; // 390
    TimeBalloonNpc *mTimeBalloonNpc; // 398 // placement for this object 004C9844
    ProjectItemDirector *mProjectItemDirector; // 3A0
    Pyramid *mPyramid; // 3A8
    OpeningStageStartDemo *mOpeningStageStartDemo; // 3B0 // can also be DemoStartWorldWaterfallStage
    __int64_t nerveExeDemoStageStart; // 3B8
    SceneAudioSystemPauseController *mSceneAudioSystemPauseController; // 3C0
    DemoSoundSynchronizer *mDemoSoundSynchronizer; // 3C8
    al::SimpleAudioUser *mAudioStageStartAtmosSe; // 3D0
    al::SimpleAudioUser *mAudioSePlayArea; // 3D8
    al::SimpleAudioUser *mAudioSnapShotCameraCtrl; // 3E0
    ProjectSeNamedList *mProjectSeNamedList; // 3E8
    _BYTE gap_3F0[0x8]; // 3F0
    TimeBalloonDirector *mTimeBalloonDirector; // 3F8
    TimeBalloonSequenceInfo *mTimeBalloonSequenceInfo; // 400
    __int64_t unkSomethingElse; // 408
    al::CameraTicket *mWarpCheckpointCamera; // 410
    sead::Vector3f *mWarpToCheckpointCamera; // 418
    sead::Vector3f *mWarpToCheckpointCameraPoint; // 420
    sead::Vector3f *mWarpToCheckpointPlayerPos; // 428
    sead::Vector3f mWarpToCheckpointDestPoint; // 430 to 43C
    _BYTE padding_43C[0x4]; // 43C
    al::LiveActor *mCheckpointFlagWarp; // 440
    al::ParabolicPath *mParabolicPath; // 448
    KoopaHackFunction *mKoopaHackFunction; // 450
    al::LiveActor *mShineDot; // 458
    _BYTE gap_460[0x10]; // 460
    CollectBgmRegister *mCollectBgmRegister; // 470
    BgmAnimeSyncDirector *mBgmAnimeSyncDirector; // 478
    al::DemoSyncedEventKeeper *mDemoSyncedEventKeeper; // 480
    char *activeDemoName; // 488
    char *activeDemoType; // 490 // maybe wrong
	NpcEventDirector *mNpcEventDirector; // 498
    al::ChromakeyDrawer *mChromakeyDrawer; // 4A0
    al::HtmlViewer *mHtmlViewer; // 4A8
    al::NfpDirector *mNfpDirector; // 4B0
    HelpAmiiboDirector *mHelpAmiiboDirector; // 4B8
};