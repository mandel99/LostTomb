#ifndef PHARAOH_PLATFORM_WINDOWS_CITY_UI_H
#define PHARAOH_PLATFORM_WINDOWS_CITY_UI_H
#include "pharaoh/rendering/image_draw.h"
#include "pharaoh/scenario/saved_scene_storage.h"
#include "pharaoh/ui/build_menu.h"
#include "pharaoh/runtime/city_view.h"

struct PharaohWindowsStartupHost;
struct PharaohWindowsCityUi;
typedef struct PharaohWindowsCityUiScene {
    const PharaohSavedSceneStorage *saved;
    const PharaohCityViewContext *view;
    const PharaohBuildMenuContext *build_menu;
    const int32_t *overlay,*message_count,*problem_count;
    uint16_t *tint; /* actual mutable70E120, distinct from font color */
} PharaohWindowsCityUiScene;
/* Actual locked frame and live scene inputs. The current scene adapter
 * supports the normal initial mission with overlay0; unsupported modes fail. */
int32_t pharaoh_windows_city_ui_begin_frame(struct PharaohWindowsStartupHost *,
    PharaohImageDrawContext *,const PharaohWindowsCityUiScene *,
    const uint8_t *window_id,const int32_t *image_group_first);
void pharaoh_windows_city_ui_draw_foreground(struct PharaohWindowsStartupHost *);
void pharaoh_windows_city_ui_release(struct PharaohWindowsStartupHost *);
#endif
