#ifndef PHARAOH_PLATFORM_WINDOWS_CITY_UI_H
#define PHARAOH_PLATFORM_WINDOWS_CITY_UI_H
#include "pharaoh/rendering/image_draw.h"
#include "pharaoh/scenario/saved_scene_storage.h"
#include "pharaoh/ui/build_menu.h"
#include "pharaoh/runtime/city_view.h"

struct PharaohWindowsStartupHost;
struct PharaohWindowsCityUi;
struct PharaohMissionScene;
struct PharaohRichTextState;
struct PharaohCityRenderContext;
typedef struct PharaohWindowsCityUiScene {
    const PharaohSavedSceneStorage *saved;
    const PharaohCityViewContext *view;
    const PharaohBuildMenuContext *build_menu;
    const int32_t *overlay,*message_count,*problem_count;
    uint16_t *tint; /* actual mutable70E120, distinct from font color */
    struct PharaohMissionScene *scene;
    struct PharaohRichTextState *text_metrics; /* shared briefing/UI E207xx */
} PharaohWindowsCityUiScene;
/* Actual locked frame and live scene inputs. The current scene adapter
 * supports the normal initial mission with overlay0; unsupported modes fail. */
int32_t pharaoh_windows_city_ui_begin_frame(struct PharaohWindowsStartupHost *,
    PharaohImageDrawContext *,const PharaohWindowsCityUiScene *,
    uint8_t *window_id,const int32_t *image_group_first);
void pharaoh_windows_city_ui_draw_foreground(struct PharaohWindowsStartupHost *);
/* Complete normal, nonmodal Window2 owner: Native51FD60 over the live city,
 * sidebar, image tables, viewport and startup-measured E39104 duration. */
uint8_t pharaoh_windows_city_ui_sidebar_slide(
    struct PharaohWindowsStartupHost *,struct PharaohCityRenderContext *);
void pharaoh_windows_city_ui_release(struct PharaohWindowsStartupHost *);
int32_t pharaoh_windows_city_ui_navigation(struct PharaohWindowsStartupHost *,
    int32_t mouse_x,int32_t mouse_y,uint8_t left_down,uint8_t right_went_down);
/* Actual Native empty-terrain path; unsupported building/walker/action
 * providers are reported as unavailable, never replaced by success callbacks.
 * open returns-1 for an unavailable domain;0/1 are the actual initializer result. */
int32_t pharaoh_windows_city_ui_open_empty_terrain(struct PharaohWindowsStartupHost *,
    struct PharaohMissionScene *,const int32_t *tile_x,const int32_t *tile_y);
int32_t pharaoh_windows_city_ui_empty_info_input(struct PharaohWindowsStartupHost *);
int32_t pharaoh_windows_city_ui_draw_empty_info(struct PharaohWindowsStartupHost *,
    struct PharaohCityRenderContext *);
#endif
