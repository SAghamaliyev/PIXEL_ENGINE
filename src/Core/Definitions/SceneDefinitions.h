#pragma once

#include <unordered_map>

#include "Core/Definitions/CameraDefinitions.h"
#include "Core/Definitions/EntityDefinitions.h"

// Data about the whole scene which will be passed to Render System
struct SceneInfo {
    const std::unordered_map<long, EntityUnit>& entityList;
    long activeEntityID;

    const std::unordered_map<long, CameraUnit>& cameraList;
    long activeCameraID;
};
