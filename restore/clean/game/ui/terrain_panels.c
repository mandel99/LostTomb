#include "pharaoh/ui/terrain_panels.h"

enum {
    TEXT_TITLE = 0, TEXT_DESCRIPTION = 1, DITCH_WATERED = 1, DITCH_DRY = 2,
    GROUP_GARDENS = 79, GROUP_PLAZA = 137, GROUP_RUBBLE = 140,
    GROUP_IRRIGATION_DITCH = 141, TERRAIN_FROM_BOTTOM = 158, PLAZA_FROM_BOTTOM = 113,
    RUIN_TYPE_FROM_BOTTOM = 173, RUIN_TEXT_FROM_BOTTOM = 143,
    DITCH_BLOCKS_FROM_BOTTOM = 8
};

static int32_t from_bottom(const PharaohTerrainPanelContext *c, int32_t value)
{
    return c->panel_height_blocks * c->block_height - value + c->panel_y;
}

static void draw_terrain(PharaohTerrainPanelContext *c, int32_t help, int32_t group)
{
    c->help_id = c->help_messages[help]; c->first_draw = 0;
    c->draw_title(c->user_data, group, TEXT_TITLE);
    c->draw_text(c->user_data, group, TEXT_DESCRIPTION,
        from_bottom(c, TERRAIN_FROM_BOTTOM));
}

void pharaoh_draw_gardens_panel(PharaohTerrainPanelContext *c)
{ draw_terrain(c, c->help_gardens, GROUP_GARDENS); }

void pharaoh_draw_canal_panel(PharaohTerrainPanelContext *c)
{
    c->help_id = c->help_messages[c->help_canal]; c->first_draw = 0;
    c->draw_title(c->user_data, GROUP_IRRIGATION_DITCH, TEXT_TITLE);
    c->draw_text(c->user_data, GROUP_IRRIGATION_DITCH,
        c->canal_has_water != 0 ? DITCH_WATERED : DITCH_DRY,
        (c->panel_height_blocks - DITCH_BLOCKS_FROM_BOTTOM) * c->block_height +
            c->panel_y);
}

void pharaoh_draw_plaza_panel(PharaohTerrainPanelContext *c)
{
    c->help_id = c->help_messages[c->help_plaza]; c->first_draw = 0;
    c->prepare_walker_list(c->user_data);
    c->draw_title(c->user_data, GROUP_PLAZA, TEXT_TITLE);
    c->draw_walker_list(c->user_data);
    c->draw_text(c->user_data, GROUP_PLAZA, TEXT_DESCRIPTION,
        from_bottom(c, PLAZA_FROM_BOTTOM));
}

void pharaoh_draw_ruins_panel(PharaohTerrainPanelContext *c)
{
    c->help_id = c->help_messages[c->help_ruins]; c->first_draw = 0;
    c->draw_title(c->user_data, GROUP_RUBBLE, TEXT_TITLE);
    c->draw_building_type(c->user_data, c->rubble_building_type,
        c->panel_x + c->text_x, from_bottom(c, RUIN_TYPE_FROM_BOTTOM));
    c->draw_text(c->user_data, GROUP_RUBBLE, TEXT_DESCRIPTION,
        from_bottom(c, RUIN_TEXT_FROM_BOTTOM));
}
