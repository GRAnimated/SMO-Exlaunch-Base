#pragma once
#include "al/camera/CameraPoser.h"

class CameraPoserFollowLimit : public al::CameraPoser {
    public:
        unsigned char massive[0x60];

        //sead::Vector3f lookAtPos; // 0x1F4
};