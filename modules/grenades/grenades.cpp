#include "../modules.hpp"
#include "../../direct_x/direct_x.h"
#include "../../overlay/duplication/duplication.h"
#include <cmath>
#include <vector>

// Simple grenade trajectory prediction
namespace grenades {

struct TrajectoryPoint {
    float x, y, z;
};

std::vector<TrajectoryPoint> g_trajectoryPoints;
std::vector<TrajectoryPoint> g_bouncePoints;

void simulateGrenadeTrajectory(Vector3 startPos, Vector3 velocity, std::vector<TrajectoryPoint>& outPoints, std::vector<TrajectoryPoint>& outBounces) {
    outPoints.clear();
    outBounces.clear();

    const float gravity = 800.0f;
    const float tickInterval = 0.015f;
    const int maxTicks = 200;
    const float elasticity = 0.45f;
    const float stopSpeed = 20.0f;

    Vector3 pos = startPos;
    Vector3 vel = velocity;

    for (int tick = 0; tick < maxTicks; ++tick) {
        // Add point every few ticks
        if (tick % 4 == 0) {
            Vector2 screen;
            if (WorldToScreen(viewMatrix, pos, screen, 1920, 1080)) {
                outPoints.push_back({ screen.x, screen.y, pos.z });
            }
        }

        // Apply gravity
        vel.z -= gravity * tickInterval;

        // Move position
        pos.x += vel.x * tickInterval;
        pos.y += vel.y * tickInterval;
        pos.z += vel.z * tickInterval;

        // Simple ground bounce detection
        if (pos.z < 0) {
            pos.z = 0;
            vel.z = -vel.z * elasticity;
            
            Vector2 bounceScreen;
            if (WorldToScreen(viewMatrix, pos, bounceScreen, 1920, 1080)) {
                outBounces.push_back({ bounceScreen.x, bounceScreen.y, pos.z });
            }

            // Stop if too slow
            if (std::fabs(vel.z) < stopSpeed) {
                vel.z = 0;
            }
        }

        // Check if stopped
        if (std::fabs(vel.x) < stopSpeed && std::fabs(vel.y) < stopSpeed && std::fabs(vel.z) < stopSpeed) {
            break;
        }
    }
}

void handler(RECT rect) {
    if (!grenades::enabled) return;

    int gameW = rect.right - rect.left;
    int gameH = rect.bottom - rect.top;

    // Get local player info (simplified - would need actual memory reading)
    // For now, this is a placeholder that shows the trajectory system
    
    // Draw stored trajectory points
    if (!g_trajectoryPoints.empty()) {
        for (size_t i = 0; i + 1 < g_trajectoryPoints.size(); ++i) {
            float x1 = g_trajectoryPoints[i].x + rect.left;
            float y1 = g_trajectoryPoints[i].y + rect.top;
            float x2 = g_trajectoryPoints[i + 1].x + rect.left;
            float y2 = g_trajectoryPoints[i + 1].y + rect.top;

            float alpha = grenades::color.a * (1.0f - (float)i / g_trajectoryPoints.size());
            DX_DrawLine(dx, x1, y1, x2, y2, 
                grenades::color.r, grenades::color.g, grenades::color.b, 
                static_cast<uint8_t>(alpha), 1.5f);
        }
    }

    // Draw bounce points
    for (auto& bounce : g_bouncePoints) {
        float x = bounce.x + rect.left;
        float y = bounce.y + rect.top;
        DX_DrawRect(dx, x - 3, y - 3, 6, 6, 
            grenades::color.r, grenades::color.g, grenades::color.b, 
            grenades::color.a, 1.0f);
    }
}

} // namespace grenades
