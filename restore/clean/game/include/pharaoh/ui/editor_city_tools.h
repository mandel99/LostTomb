#ifndef PHARAOH_UI_EDITOR_CITY_TOOLS_H
#define PHARAOH_UI_EDITOR_CITY_TOOLS_H

#include "pharaoh/ui/city_ingame_dispatch.h"
#include "pharaoh/scenario/scene_routing.h"

typedef struct PharaohEditorCityToolsContext {
    const PharaohConstructionInputContext *construction;
    PharaohBuildMenuContext *menu;
    uint32_t *button_parameter;
    uint32_t *preview_mode;
    int32_t *editor_category;
    const PharaohCityBuildPopupSize *popup_sizes;
    int32_t *popup_count,*popup_y_offset,*popup_width;
    PharaohSceneRouting *routing;
} PharaohEditorCityToolsContext;

/* Whole Native409AC0/409CF0. Popup arms return before drag reset exactly as
 * in the executable; action13's later tool remap remains unreachable. */
void pharaoh_editor_city_tool_select(const PharaohEditorCityToolsContext *);
void pharaoh_editor_city_preview_select(const PharaohEditorCityToolsContext *);

#endif
