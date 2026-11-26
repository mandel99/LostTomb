#ifndef PHARAOH_PLATFORM_WINDOWS_CITY_UI_H
#define PHARAOH_PLATFORM_WINDOWS_CITY_UI_H
#include "pharaoh/rendering/image_draw.h"
#include "pharaoh/scenario/saved_scene_storage.h"

struct PharaohWindowsStartupHost;
struct PharaohWindowsCityUi;
/* Requires the actual locked game frame. Owns process UI state; saved
 * calendar/player/name slots remain borrowed, live scene data. */
int32_t pharaoh_windows_city_ui_draw_topbar(struct PharaohWindowsStartupHost *,
    PharaohImageDrawContext *,const PharaohSavedSceneStorage *,
    const uint8_t *window_id,const int32_t *event_54_suppress_flood,
    const int32_t *image_group_first);
void pharaoh_windows_city_ui_release(struct PharaohWindowsStartupHost *);
#endif
