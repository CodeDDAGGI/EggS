#ifndef _struct_device_cmd_hpp
#define _struct_device_cmd_hpp

enum class CameraMode {
    NONE,
    MODE_APPEARANCE, // 외관
    MODE_ILLUMINATION, // 투관
    MODE_COMBINED // 투관 + 외관
};

struct DEVICE_CONTROL_CMD 
{
    CameraMode cameraModes[4];   // 각 카메라의 모드
    bool cameraStatus[4];        // 각 카메라의 상태 (ON/OFF)

    DEVICE_CONTROL_CMD() {
        for (int i = 0; i < 4; ++i) {
            cameraModes[i] = CameraMode::NONE;
            cameraStatus[i] = false;
        }
    }
};

#endif
