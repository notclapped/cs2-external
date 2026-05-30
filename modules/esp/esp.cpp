#include "modules.hpp"
#include "../../direct_x/direct_x.h"
#include "../../overlay/duplication/duplication.h"
#include "../../gui/imgui/imgui.h"

// Bone indices for skeleton
struct Bone {
    float x, y, z;
};

// Skeleton bone connections (bone index pairs)
const int skeletonBones[][2] = {
    {7, 6},   // Head to Neck
    {6, 4},   // Neck to Chest
    {4, 3},   // Chest to Spine
    {3, 2},   // Spine to Hips
    {2, 1},   // Hips to Pelvis
    {4, 8},   // Chest to Right Shoulder
    {8, 9},   // Right Shoulder to Right Arm
    {9, 10},  // Right Arm to Right Elbow
    {10, 11}, // Right Elbow to Right Hand
    {4, 12},  // Chest to Left Shoulder
    {12, 13}, // Left Shoulder to Left Arm
    {13, 14}, // Left Arm to Left Elbow
    {14, 15}, // Left Elbow to Left Hand
    {1, 17},  // Hips to Right Leg
    {17, 18}, // Right Leg to Right Knee
    {18, 19}, // Right Knee to Right Foot
    {1, 20},  // Hips to Left Leg
    {20, 21}, // Left Leg to Left Knee
    {21, 22}  // Left Knee to Left Foot
};

void drawSkeleton(RECT rect, int gameW, int gameH, PlayerData& p, zdraw::rgba color) {
    if (!p.bonesValid) return;

    for (int i = 0; i < 19; i++) {
        int bone1Idx = skeletonBones[i][0];
        int bone2Idx = skeletonBones[i][1];

        if (bone1Idx >= 27 || bone2Idx >= 27) continue;

        Vector3 bone1Pos = { p.bones[bone1Idx].x, p.bones[bone1Idx].y, p.bones[bone1Idx].z };
        Vector3 bone2Pos = { p.bones[bone2Idx].x, p.bones[bone2Idx].y, p.bones[bone2Idx].z };

        Vector2 screen1, screen2;
        if (!WorldToScreen(viewMatrix, bone1Pos, screen1, gameW, gameH)) continue;
        if (!WorldToScreen(viewMatrix, bone2Pos, screen2, gameW, gameH)) continue;

        screen1.x += rect.left; screen1.y += rect.top;
        screen2.x += rect.left; screen2.y += rect.top;

        DX_DrawLine(dx, screen1.x, screen1.y, screen2.x, screen2.y,
            color.r, color.g, color.b, color.a, 1.0f);
    }
}

void esp::handler(RECT rect){
    int gameW = rect.right - rect.left;
    int gameH = rect.bottom - rect.top;

    for(auto& p : g_players){
        if(p.team == localTeam && !esp::teams) continue;

        Vector3 feetPos = { p.pos.x, p.pos.y, p.pos.z - 8.0f  };
        Vector3 headPos = { p.pos.x, p.pos.y, p.pos.z + 70.0f };

        Vector2 screenFeet, screenHead;
        if(!WorldToScreen(viewMatrix, feetPos, screenFeet, gameW, gameH)) continue;
        if(!WorldToScreen(viewMatrix, headPos, screenHead, gameW, gameH)) continue;

        screenFeet.x += rect.left; screenFeet.y += rect.top;
        screenHead.x += rect.left; screenHead.y += rect.top;

        float h = screenFeet.y - screenHead.y;
        if(h <= 5 || h > 500) continue;

        float w = h / 2.5f;
        float x = screenHead.x - w / 2.0f;
        float y = screenHead.y;

        // Draw skeleton first (behind everything)
        if (esp::enableSkeleton) {
            drawSkeleton(rect, gameW, gameH, p, esp::skeletonColor);
        }

        if(esp::enableBoxes){
            // Corner style box (cleaner look from TEMP source)
            float cornerLen = w * 0.15f;
            
            // Top corners
            DX_DrawLine(dx, x, y, x + cornerLen, y, esp::color.r, esp::color.g, esp::color.b, esp::color.a, 1.5f);
            DX_DrawLine(dx, x + w, y, x + w - cornerLen, y, esp::color.r, esp::color.g, esp::color.b, esp::color.a, 1.5f);
            // Bottom corners
            DX_DrawLine(dx, x, y + h, x + cornerLen, y + h, esp::color.r, esp::color.g, esp::color.b, esp::color.a, 1.5f);
            DX_DrawLine(dx, x + w, y + h, x + w - cornerLen, y + h, esp::color.r, esp::color.g, esp::color.b, esp::color.a, 1.5f);
            // Left corners
            DX_DrawLine(dx, x, y, x, y + cornerLen, esp::color.r, esp::color.g, esp::color.b, esp::color.a, 1.5f);
            DX_DrawLine(dx, x, y + h, x, y + h - cornerLen, esp::color.r, esp::color.g, esp::color.b, esp::color.a, 1.5f);
            // Right corners
            DX_DrawLine(dx, x + w, y, x + w, y + cornerLen, esp::color.r, esp::color.g, esp::color.b, esp::color.a, 1.5f);
            DX_DrawLine(dx, x + w, y + h, x + w, y + h - cornerLen, esp::color.r, esp::color.g, esp::color.b, esp::color.a, 1.5f);
        }

        if(esp::enableHealthESP){
            float barH = h * (p.health * 0.01f);
            float barX = x - 5;
            // Background
            DX_DrawLine(dx, barX, y, barX, y + h, 50, 50, 50, 200, 3.0f);
            // Health bar with gradient effect
            DX_DrawLine(dx, barX, y + h, barX, y + h - barH, esp::healthColor.r, esp::healthColor.g, esp::healthColor.b, esp::healthColor.a, 3.0f);
        }

        if(esp::enableNameESP){
            float textW = ImGui::CalcTextSize(p.name).x;
            float textX = x + w / 2.0f - textW / 2.0f;
            // Shadow effect
            DX_DrawText(textX - 1, y - 16, p.name, 0, 0, 0, 255);
            DX_DrawText(textX + 1, y - 16, p.name, 0, 0, 0, 255);
            DX_DrawText(textX,     y - 17, p.name, 0, 0, 0, 255);
            DX_DrawText(textX,     y - 15, p.name, 0, 0, 0, 255);
            // Main text
            DX_DrawText(textX,     y - 16, p.name, esp::nameColor.r, esp::nameColor.g, esp::nameColor.b, esp::nameColor.a);
        }
    }
}