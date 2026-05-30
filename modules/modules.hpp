// modules.hpp
#pragma once
#include "../memory/memory.h"
#include "../gui/zdraw/zdraw/zdraw.hpp"
#include <atomic>

extern float viewMatrix[16];
extern std::string configName;

struct QAngle {
    float x; // Pitch
    float y; // Yaw
    float z; // Roll
};

namespace reader {
    void handler();
}

namespace esp {
    void handler(RECT rect);
    
    extern bool enabled;
    extern bool enableNameESP;
    extern bool enableHealthESP;
    extern bool enableBoxes;
    extern bool enableSkeleton;
    extern bool borders;
    extern bool teams;

    extern zdraw::rgba color;
    extern zdraw::rgba nameColor;
    extern zdraw::rgba healthColor;
    extern zdraw::rgba skeletonColor;
}

namespace tracers {
    void handler(RECT rect);
    extern float offset;
    extern bool enabled;
    extern int style;
    extern zdraw::rgba color;
    extern bool teams;
    extern int centered;
}

namespace grenades {
    void handler(RECT rect);
    extern bool enabled;
    extern bool localOnly;
    extern zdraw::rgba color;
}
