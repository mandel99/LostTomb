#ifndef PHARAOH_PLATFORM_WINDOWS_MISSION_RUNTIME_H
#define PHARAOH_PLATFORM_WINDOWS_MISSION_RUNTIME_H
#include "pharaoh/platform/windows_startup_host.h"
#include "pharaoh/scenario/mission_scene.h"

typedef struct PharaohWindowsMissionRuntime PharaohWindowsMissionRuntime;
PharaohWindowsMissionRuntime *pharaoh_windows_mission_runtime_create(
    PharaohWindowsStartupHost *host,PharaohBriefingState *briefing,uint8_t *window);
void pharaoh_windows_mission_runtime_destroy(PharaohWindowsMissionRuntime *runtime);
void pharaoh_windows_mission_runtime_briefing(PharaohWindowsMissionRuntime *runtime,
    int32_t mouse_x,int32_t mouse_y,uint8_t left_pressed,uint8_t right_pressed,
    uint8_t left_down);
int32_t pharaoh_windows_mission_runtime_game_frame(
    PharaohWindowsMissionRuntime *runtime,int32_t mouse_x,int32_t mouse_y,
    uint8_t left_down,uint8_t right_went_down);
const PharaohMissionScene *pharaoh_windows_mission_runtime_scene(
    const PharaohWindowsMissionRuntime *runtime);
/* Process-launch diagnostic: same briefing load and actual continue owner. */
int32_t pharaoh_windows_mission_runtime_diagnostic_frame_status(const PharaohWindowsMissionRuntime *runtime);
int32_t pharaoh_windows_mission_runtime_diagnostic_begin_game(PharaohWindowsMissionRuntime *);
#endif
