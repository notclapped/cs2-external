#include "modules.hpp"
#include "../gui/zdraw/zdraw/zdraw.hpp"

std::string configName = "config";

namespace esp {
    bool enabled = false;
    bool enableNameESP = false;
    bool enableHealthESP = false;
    bool enableBoxes = false;
    bool enableSkeleton = false;
    bool borders = false;
    bool teams = false;

    zdraw::rgba color = {255, 255, 255, 255};
    zdraw::rgba nameColor = {255, 255, 255, 255};
    zdraw::rgba healthColor = {0, 255, 0, 255};
    zdraw::rgba skeletonColor = {170, 175, 220, 255};
}

namespace tracers {
    bool enabled = false;
    int style = 0;
    float offset = 80;
    zdraw::rgba color = {255, 255, 255, 255};
    bool teams = false;
    int centered = true;
}

namespace grenades {
    bool enabled = false;
    bool localOnly = true;
    zdraw::rgba color = {170, 175, 220, 200};
}
