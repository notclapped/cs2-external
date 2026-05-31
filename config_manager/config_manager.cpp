#include "json.hpp"
#include "config_manager.hpp"
#include "../modules/modules.hpp"
#include <fstream>
#include <cstdio>
#include <windows.h>
#include <filesystem>
#include <shlobj.h>

using json = nlohmann::json;
namespace fs = std::filesystem;

static std::string getConfigPath(const std::string& filename) {
    char documents[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documents);
    
    std::string path = std::string(documents) + "\\gamingchair\\";
    fs::create_directories(path);
    
    return path + filename;
}

static json rgba_to_json(const zdraw::rgba& c) {
    return { c.r, c.g, c.b, c.a };
}

static zdraw::rgba json_to_rgba(const json& j, zdraw::rgba def) {
    if (!j.is_array() || j.size() < 4) return def;
    return { j[0], j[1], j[2], j[3] };
}

std::vector<std::string> config::getConfigs() {
    std::vector<std::string> configs;

    char documents[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documents);
    std::string path = std::string(documents) + "\\gamingchair\\";

    if (!fs::exists(path)) return configs;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".json")
            configs.push_back(entry.path().filename().string());
    }

    return configs;
}

int config::save(const std::string& filename){
    std::string fullPath = getConfigPath(filename);
    json j;

    j["esp"]        = esp::enabled;
    j["espName"]    = esp::enableNameESP; 
    j["espHealth"]  = esp::enableHealthESP;
    j["espBoxes"]   = esp::enableBoxes;
    j["espTeams"]   = esp::teams;

    j["espColor"]       = rgba_to_json(esp::color);
    j["espNameColor"]   = rgba_to_json(esp::nameColor);
    j["espHealthColor"] = rgba_to_json(esp::healthColor);

    j["tracers"]         = tracers::enabled;
    j["tracersStyle"]    = tracers::style;
    j["tracersOffset"]   = tracers::offset;
    j["tracersTeams"]    = tracers::teams;
    j["tracersCentered"] = tracers::centered;

    j["tracersColor"] = rgba_to_json(tracers::color);

    j["bones"]          = bones::enabled;
    j["bonesHead"]      = bones::head;
    j["bonesHeadFilled"]= bones::filled;
    j["bonesTeams"]     = bones::teams;

    j["bonesColor"]       = rgba_to_json(bones::color);
    j["bonesHeadColor"]   = rgba_to_json(bones::headColor);
    j["bonesFilledColor"] = rgba_to_json(bones::filledColor);

    j["aim"]             = aimassist::enabled;
    j["aimRenderRadio"]  = aimassist::renderRadio;
    j["aimRenderDeadZone"]= aimassist::renderDeadZone;
    j["aimRadio"]        = aimassist::radio;
    j["aimDeadZoneEnabled"] = aimassist::deadZoneEnabled;
    j["aimDeadZone"]     = aimassist::deadZone;
    j["aimSmoothing"]    = aimassist::smoothing;

    j["trigger"]       = triggerbot::enabled;
    j["triggerDelay"]  = triggerbot::delay;
    j["triggerRandom"] = triggerbot::randomization;

    std::ofstream file(fullPath);
    if (!file.is_open()) return 0; 

    file << j.dump(4);
    file.close();

    return 1; 
}

int config::load(const std::string& filename){
    std::string fullPath = getConfigPath(filename);
    std::ifstream file(fullPath); 

    if (!file.is_open()) return 1;

    json j;
    file >> j;
    file.close();

    esp::enabled          = j.value("esp", false);
    esp::enableNameESP    = j.value("espName", false);
    esp::enableHealthESP  = j.value("espHealth", false);
    esp::enableBoxes      = j.value("espBoxes", false);
    esp::enableSkeleton   = j.value("espSkeleton", false);
    esp::borders          = j.value("espBorders", false);
    esp::teams            = j.value("espTeams", false);

    esp::color        = json_to_rgba(j.value("espColor",        json::array({255,255,255,255})), {255,255,255,255});
    esp::nameColor    = json_to_rgba(j.value("espNameColor",    json::array({255,255,255,255})), {255,255,255,255});
    esp::healthColor  = json_to_rgba(j.value("espHealthColor",  json::array({0,255,0,255})),     {0,255,0,255});
    esp::skeletonColor= json_to_rgba(j.value("espSkeletonColor",json::array({170,175,220,255})), {170,175,220,255});

    tracers::enabled     = j.value("tracers", false);
    tracers::style       = j.value("tracersStyle", 0);
    tracers::offset      = j.value("tracersOffset", 0.0f);
    tracers::teams       = j.value("tracersTeams", false);
    tracers::centered    = j.value("tracersCentered", 0);

    tracers::color = json_to_rgba(j.value("tracersColor", json::array({255,255,255,255})), {255,255,255,255});

    grenades::enabled   = j.value("grenades", false);
    grenades::localOnly = j.value("grenadesLocal", true);
    grenades::color     = json_to_rgba(j.value("grenadesColor", json::array({170,175,220,200})), {170,175,220,200});

    return 0;
}

int config::del(const std::string& filename){
    return std::remove(getConfigPath(filename).c_str()) == 0;
}