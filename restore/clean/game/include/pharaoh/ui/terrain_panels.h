#ifndef PHARAOH_UI_TERRAIN_PANELS_H
#define PHARAOH_UI_TERRAIN_PANELS_H

#include <stdint.h>

typedef void (*PharaohPanelTitleFunction)(
    void *user_data, int32_t group, int32_t text);
typedef void (*PharaohPanelTextFunction)(
    void *user_data, int32_t group, int32_t text, int32_t y);
typedef void (*PharaohPanelSimpleFunction)(void *user_data);
typedef void (*PharaohPanelBuildingTypeFunction)(void *user_data,
    int32_t building_type, int32_t x, int32_t y);

typedef struct PharaohTerrainPanelContext {
    int32_t panel_x;
    int32_t panel_y;
    int32_t panel_height_blocks;
    int32_t block_height;
    int32_t text_x;
    int32_t canal_has_water;
    int32_t rubble_building_type;
    int32_t help_gardens;
    int32_t help_canal;
    int32_t help_plaza;
    int32_t help_ruins;
    int32_t help_id;
    int32_t first_draw;
    const int32_t *help_messages;
    PharaohPanelTitleFunction draw_title;
    PharaohPanelTextFunction draw_text;
    PharaohPanelSimpleFunction prepare_walker_list;
    PharaohPanelSimpleFunction draw_walker_list;
    PharaohPanelBuildingTypeFunction draw_building_type;
    void *user_data;
} PharaohTerrainPanelContext;

void pharaoh_draw_gardens_panel(PharaohTerrainPanelContext *context);
void pharaoh_draw_canal_panel(PharaohTerrainPanelContext *context);
void pharaoh_draw_plaza_panel(PharaohTerrainPanelContext *context);
void pharaoh_draw_ruins_panel(PharaohTerrainPanelContext *context);

#endif
