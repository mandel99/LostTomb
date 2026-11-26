#include "pharaoh/ui/debug_info.h"
#include "pharaoh/ui/message_actions.h"
#include <stddef.h>

static void center(const PharaohDebugInfoText *c, const char *text,
    int32_t x, int32_t y, int32_t width, int32_t font, uint32_t color)
{
    PharaohBitmapTextCenteredNativeContext drawing = *c->centered;
    if (font == *c->small_image) drawing.font = c->small_font;
    pharaoh_bitmap_text_draw_centered_native(&drawing,
        (const uint8_t *)text, x, y, width, font, (uint16_t)color);
}
static void number(const PharaohDebugInfoText *c, int32_t value, char prefix,
    const char *suffix, int32_t x, int32_t y, int32_t font, uint32_t color,
    int32_t origin_x, int32_t origin_y)
{
    pharaoh_ui_text_draw_number(c->ui,c->small_font,value,prefix,suffix,
        x,y,font,(uint16_t)color,origin_x,origin_y);
}
static const char sEvolveInfoF3005F8C58[] = "Evolve info (F3)";
static const char sBarb005F8BC8[] = " = barb";
static const char sEver005F8C18[] = " = ever";
static const char sEvolveError005F8BDC[] = " = evolve error";
static const char sLastBuilding005F8C20[] = " = last building";
static const char sMonth005F8C3C[] = " = month";
static const char sNoOfConnects005F8C04[] = " = no of connects";
static const char sRoutings005F8BD0[] = " = routings";
static const char sTick005F8C50[] = " = tick";
static const char sTotalConnections005F8BEC[] = " = total connections";
static const char sWeek005F8C48[] = " = week";
static const char sYear005F8C34[] = " = year";
static const char field005E0400[] = ",";
static const char field005F8EC0[] = "x,y";
static const char sFigs005F8EF4[] = " Figs";
static const char sWalkerInfoF1005F8F14[] = "Figure info (F1)";
static const char sNotWatchingAWalker005F8EFC[] = "Not watching a figure";
static const char sAgentFor005F8E98[] = " = agent_for ";
static const char sDockers005F8E60[] = " = dockers ";
static const char sWalkerNo005F8ED0[] = " = figure no";
static const char sGhost005F8E6C[] = " = ghost ";
static const char sImmigrantTo005F8E84[] = " = immigrant_to ";
static const char sPeople005F8E78[] = " = people ";
static const char sRuler005F8EC4[] = " = ruler";
static const char sFreeRouteSlots005F8EE0[] = " free route slots";
static const char sMapXY005F8EB4[] = " map x,y";
static const char sXYBits005F8EA8[] = "x,y bits";

/* Complete existing C reuse: native00534DF0. */
void pharaoh_debug_evolution_draw(const PharaohDebugEvolutionContext *s)
{
  const PharaohDebugInfoText *const c=s->text;
  int32_t panelY;
  int32_t errorColor;

  if ((*s->enabled) == 1) {
    panelY = (*c->origin_y) * 2;
    pharaoh_panel_blocks_draw(c->panel,6,0,panelY + 0xdc,0xc,0xf,1);
    center(c,sEvolveInfoF3005F8C58,0,panelY + 0xe2,0xc0,(*c->normal_image),0xffff);
    number(c,(*s->tick),0x40,sTick005F8C50,8,panelY + 0xf6,(*c->small_image),0x3efa,(*c->origin_x)
                 ,(*c->origin_y));
    number(c,(*s->day),0x40,sWeek005F8C48,0x6e,panelY + 0xf6,(*c->small_image),0x3efa,
                 (*c->origin_x),(*c->origin_y));
    number(c,(*s->month),0x40,sMonth005F8C3C,8,panelY + 0x104,(*c->small_image),0x3efa,
                 (*c->origin_x),(*c->origin_y));
    number(c,(*s->year),0x40,sYear005F8C34,0x6e,panelY + 0x104,(*c->small_image),0x3efa,
                 (*c->origin_x),(*c->origin_y));
    number(c,(*s->building_bound),0x40,sLastBuilding005F8C20,8,panelY + 0x112,(*c->small_image),0x3efa,
                 (*c->origin_x),(*c->origin_y));
    number(c,(*s->buildings_ever),0x40,sEver005F8C18,0x6e,panelY + 0x112,(*c->small_image),0x3efa,
                 (*c->origin_x),(*c->origin_y));
    number(c,(*s->connection_count) + -1,0x40,sNoOfConnects005F8C04,8,panelY + 0x120,(*c->small_image),
                 0x3efa,(*c->origin_x),(*c->origin_y));
    number(c,(*s->total_connections),0x40,sTotalConnections005F8BEC,8,panelY + 0x13c,(*c->small_image),
                 0x3efa,(*c->origin_x),(*c->origin_y));
    errorColor = ((0u - (uint32_t)((*s->error) != 0)) & 0xb906) + 0x3efa;
    number(c,(*s->error),0x40,sEvolveError005F8BDC,8,panelY + 0x14a,(*c->small_image),errorColor,
                 (*c->origin_x),(*c->origin_y));
    number(c,(*s->routings),0x40,sRoutings005F8BD0,8,panelY + 0x158,(*c->small_image),errorColor,
                 (*c->origin_x),(*c->origin_y));
    number(c,(*s->barbarians),0x40,sBarb005F8BC8,8,panelY + 0x166,(*c->small_image),errorColor,(*c->origin_x)
                 ,(*c->origin_y));
  }
  return;
}

static void walker_number(const PharaohDebugInfoText *c, int32_t value,
    const char *suffix, int32_t x, int32_t y)
{
    number(c,value,'@',suffix,x,y,*c->small_image,0x3efa,
        *c->origin_x,*c->origin_y);
}

/* Complete typed reuse of native00536120, including sticky watched selection. */
void pharaoh_debug_walker_draw(const PharaohDebugWalkerContext *s)
{
    if (*s->enabled != 1) return;
    const PharaohDebugInfoText *const c = s->text;
    const int32_t panel_y = *c->origin_y * 2;
    pharaoh_panel_blocks_draw(c->panel,6,0,panel_y+0xdc,12,15,1);
    *s->selected_grid = pharaoh_city_view_mouse_to_grid_offset(s->city->state,
        s->city->projection,s->hit,*s->mouse_x,*s->mouse_y);
    if (*s->selected_grid == 0) *s->watched = 0;
    else if (s->walker_heads[*s->selected_grid] != 0)
        *s->watched = s->walker_heads[*s->selected_grid];
    center(c,sWalkerInfoF1005F8F14,0,panel_y+0xe2,192,*c->normal_image,0xffff);
    if (*s->watched == 0) {
        center(c,sNotWatchingAWalker005F8EFC,0,panel_y+0xf4,192,
            *c->normal_image,0xffff);
        walker_number(c,*s->count,sFigs005F8EF4,8,panel_y+0x10c);
        walker_number(c,*s->ever,sEver005F8C18,110,panel_y+0x10c);
        const int32_t free_routes = pharaoh_message_count_empty_slots(s->route_slots);
        walker_number(c,free_routes,sFreeRouteSlots005F8EE0,8,panel_y+0x118);
        return;
    }
    walker_number(c,*s->watched,sWalkerNo005F8ED0,8,panel_y+0xf4);
    walker_number(c,s->walkers[*s->watched].owner,sRuler005F8EC4,110,panel_y+0xf4);
    PharaohBitmapTextDrawState *const text = c->ui->drawing;
    text->cursor_x = 0;
    walker_number(c,s->walkers[*s->watched].cross_country_x,
        field005E0400,8,panel_y+0x10c);
    walker_number(c,s->walkers[*s->watched].cross_country_y,
        field005F8EC0,text->cursor_x+8,panel_y+0x10c);
    text->cursor_x = 0;
    walker_number(c,s->walkers[*s->watched].x,field005E0400,100,panel_y+0x10c);
    walker_number(c,s->walkers[*s->watched].y,sMapXY005F8EB4,
        text->cursor_x+100,panel_y+0x10c);
    text->cursor_x = 0;
    walker_number(c,(uint8_t)s->walkers[*s->watched].cross_country_x & 15,
        field005E0400,8,panel_y+0x124);
    walker_number(c,(uint8_t)s->walkers[*s->watched].cross_country_y & 15,
        sXYBits005F8EA8,text->cursor_x+8,panel_y+0x124);
    text->cursor_x = 0;
    walker_number(c,s->walkers[*s->watched].building_id,
        sAgentFor005F8E98,8,panel_y+0x130);
    walker_number(c,s->walkers[*s->watched].related_building_id_5c,
        sImmigrantTo005F8E84,text->cursor_x+8,panel_y+0x130);
    text->cursor_x = 0;
    walker_number(c,s->walkers[*s->watched].reserved_062[2],
        sPeople005F8E78,8,panel_y+0x13c);
    walker_number(c,s->walkers[*s->watched].is_ghost,
        sGhost005F8E6C,text->cursor_x+8,panel_y+0x13c);
    int32_t dockers = 0;
    for (int32_t i=1; i<2000; ++i)
        if (s->walkers[i].state == 1 && s->walkers[i].type == 38) ++dockers;
    text->cursor_x = 0;
    walker_number(c,dockers,sDockers005F8E60,8,panel_y+0x148);
}

_Static_assert(sizeof(PharaohWalkerQueryRecord) == 0x184 &&
    offsetof(PharaohWalkerQueryRecord,type) == 0x0a &&
    offsetof(PharaohWalkerQueryRecord,state) == 0x0e &&
    offsetof(PharaohWalkerQueryRecord,owner) == 0x0f &&
    offsetof(PharaohWalkerQueryRecord,cross_country_x) == 0x4a &&
    offsetof(PharaohWalkerQueryRecord,cross_country_y) == 0x4c &&
    offsetof(PharaohWalkerQueryRecord,building_id) == 0x5a &&
    offsetof(PharaohWalkerQueryRecord,related_building_id_5c) == 0x5c &&
    offsetof(PharaohWalkerQueryRecord,reserved_062) + 2 == 0x64 &&
    offsetof(PharaohWalkerQueryRecord,is_ghost) == 0x65,
    "native figure debug record views");

_Static_assert(sizeof(PharaohDebugCityPlayer) == 0x49d8 &&
    offsetof(PharaohDebugCityPlayer,active) == 2 &&
    offsetof(PharaohDebugCityPlayer,population) == 0x24 &&
    offsetof(PharaohDebugCityPlayer,capacity) == 0x34 &&
    offsetof(PharaohDebugCityPlayer,spare_capacity) == 0x38 &&
    offsetof(PharaohDebugCityPlayer,births) == 0x26dc &&
    offsetof(PharaohDebugCityPlayer,deaths) == 0x26e0 &&
    offsetof(PharaohDebugCityPlayer,docks) == 0x4460 &&
    offsetof(PharaohDebugCityPlayer,natives_restless) == 0x4594 &&
    offsetof(PharaohDebugCityPlayer,mission) == 0x459c &&
    offsetof(PharaohDebugCityPlayer,wine_sources) == 0x46a4 &&
    offsetof(PharaohHouseModel,health) == 0x24,
    "native city player and house model views");

static const char city_text_005f8bc0[] = "Pop is ";
static const char city_text_005f8bb4[] = "  tot cap  ";
static const char city_text_005f8ba4[] = "  spare cap  ";
static const char city_text_005f8b98[] = "  births  ";
static const char city_text_005f8b8c[] = "  deaths  ";
static const char city_text_005f8b7c[] = "  in houses ";
static const char city_text_005f8b6c[] = "nof_docks=  ";
static const char city_text_005f8b58[] = "docks_list=  ";
static const char city_text_005f8b40[] = "wine sources =  ";
static const char city_text_005f8b30[] = " MAP ROTATION";
static const char city_text_005f8b24[] = " map_style";
static const char city_text_005f8b0c[] = " natives_are_restless";
static const char city_text_005f8af8[] = " mission_in_place";
static const char city_text_005f8aec[] = " was_in_int";
static const char city_text_005f8ae0[] = " was_in_set";
static const char city_text_005f8ad0[] = " alert_messages";
static const char city_text_005f8ab4[] = " last_alert_message_shown";
static const char city_text_005f8a94[] = "Auto removed man mades tiles";
static const char city_text_005f8a84[] = " test_value1a";
static const char city_text_005f8a74[] = " test_value2a";
static const char city_text_005f8a64[] = " test_value3a";
static const char city_text_005f8a54[] = " test_value4a";
static const char city_text_005f8a48[] = " mouse_x";
static const char city_text_005f8a3c[] = " mouse_y";
static const char city_text_005f8a2c[] = " mouse_movement";
static const char city_text_005f8a1c[] = " side_panel_x1";
static const char city_text_005f8a04[] = " building_in_progress";
static const char city_text_0057c048[] = " ";
static const char city_text_005f8b68[] = "   ";
static const char city_text_005f8b54[] = ",  ";

static void plain(const PharaohDebugInfoText *c,const char *text,
    int32_t x,int32_t y,int32_t font,uint32_t color)
{
    c->ui->drawing->font_id=(uint32_t)font;
    pharaoh_bitmap_text_draw_native(c->small_font,c->ui->drawing,
        (const uint8_t *)text,x,y,(uint16_t)color);
}
/* Native00534610. The house recalculation intentionally uses current_player
 * inside every active player row, exactly as the executable does. */
void pharaoh_debug_city_draw(const PharaohDebugCityContext *s)
{
  const PharaohDebugInfoText *const c=s->text;
  int iVar1;
  PharaohDebugCityPlayer *player;
  int y;
  int local_4;

  if ((*s->enabled) == 1) {
    player = &s->players[1];
    y = 0x5e;
    do {
      local_4 = 0xffff;
      c->ui->drawing->cursor_x = 0;
      if (player->active == 1) {
        iVar1 = y + -0xe;
        c->ui->drawing->cursor_x = 0;
        plain(c,city_text_005f8bc0,8,iVar1,(*c->small_image),0xffff);
        number(c,player->population,'@',city_text_0057c048,c->ui->drawing->cursor_x + 8,iVar1,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        plain(c,city_text_005f8bb4,c->ui->drawing->cursor_x + 8,iVar1,(*c->small_image),0xffff);
        number(c,player->capacity,'@',city_text_0057c048,c->ui->drawing->cursor_x + 8,iVar1,(*c->small_image),0xffff,(*c->origin_x),
                   (*c->origin_y));
        plain(c,city_text_005f8ba4,c->ui->drawing->cursor_x + 8,iVar1,(*c->small_image),0xffff);
        number(c,player->spare_capacity,'@',city_text_0057c048,c->ui->drawing->cursor_x + 8,iVar1,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        plain(c,city_text_005f8b98,8,y,(*c->small_image),0xffff);
        number(c,player->births,'@',city_text_0057c048,c->ui->drawing->cursor_x + 8,y,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        plain(c,city_text_005f8b8c,c->ui->drawing->cursor_x + 8,y,(*c->small_image),0xffff);
        number(c,player->deaths,'@',city_text_0057c048,c->ui->drawing->cursor_x + 8,y,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        iVar1 = pharaoh_house_population_calculate_types(s->buildings,*s->highest_building,&s->players[*s->current_player].houses);
        if (iVar1 != player->population) {
          local_4 = 0xf800;
        }
        plain(c,city_text_005f8b7c,c->ui->drawing->cursor_x + 8,y,(*c->small_image),(uint16_t)local_4);
        number(c,iVar1,'@',city_text_0057c048,c->ui->drawing->cursor_x + 8,y,(*c->small_image),(uint16_t)local_4,
                   (*c->origin_x),(*c->origin_y));
        iVar1 = y + 0xe;
        c->ui->drawing->cursor_x = 0;
        plain(c,city_text_005f8b6c,8,iVar1,(*c->small_image),0xffff);
        number(c,(*s->dock_count),'@',city_text_005f8b68,c->ui->drawing->cursor_x + 8,iVar1,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        plain(c,city_text_005f8b58,c->ui->drawing->cursor_x + 8,iVar1,(*c->small_image),0xffff);
        local_4 = 0;
        do {
          number(c,s->players[*s->current_player].docks[local_4],'@'
                     ,city_text_005f8b54,c->ui->drawing->cursor_x + 4,iVar1,(*c->small_image),0xffff,(*c->origin_x),
                     (*c->origin_y));
          local_4 = local_4 + 1;
        } while (local_4 < 10);
        c->ui->drawing->cursor_x = 0;
        plain(c,city_text_005f8b40,8,y + 0x1c,(*c->small_image),0xffff);
        number(c,s->players[*s->current_player].wine_sources,'@',city_text_005f8b68,
                   c->ui->drawing->cursor_x + 8,y + 0x1c,(*c->small_image),0xffff,(*c->origin_x),(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        number(c,(*s->orientation),'@',city_text_005f8b30,8,y + 0x2a,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        number(c,(*s->map_style),'@',city_text_005f8b24,8,y + 0x38,(*c->small_image),0xffff,(*c->origin_x)
                   ,(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        number(c,s->players[*s->current_player].natives_restless,'@',
                   city_text_005f8b0c,8,y + 0x46,(*c->small_image),0xffff,(*c->origin_x),
                   (*c->origin_y));
        number(c,s->players[*s->current_player].mission,'@',
                   city_text_005f8af8,c->ui->drawing->cursor_x + 0x6c,y + 0x46,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        number(c,(*s->was_in_int),'@',city_text_005f8aec,8,y + 0x54,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        number(c,(*s->was_in_set),'@',city_text_005f8ae0,c->ui->drawing->cursor_x + 0x6c,y + 0x54,(*c->small_image),
                   0xffff,(*c->origin_x),(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        number(c,(*s->alert_count),'@',city_text_005f8ad0,8,y + 0x62,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        number(c,(*s->last_alert),'@',city_text_005f8ab4,c->ui->drawing->cursor_x + 0x6c,y + 0x62,
                   (*c->small_image),0xffff,(*c->origin_x),(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        number(c,(*s->removed_tiles),'@',city_text_005f8a94,8,y + 0x70,(*c->small_image),
                   0xffff,(*c->origin_x),(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        number(c,(*s->test1),'@',city_text_005f8a84,8,y + 0x7e,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        number(c,(*s->test2),'@',city_text_005f8a74,c->ui->drawing->cursor_x + 0x6c,y + 0x7e,(*c->small_image)
                   ,0xffff,(*c->origin_x),(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        number(c,(*s->test3),'@',city_text_005f8a64,8,y + 0x8c,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        number(c,(*s->test4),'@',city_text_005f8a54,c->ui->drawing->cursor_x + 0x6c,y + 0x8c,(*c->small_image)
                   ,0xffff,(*c->origin_x),(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        number(c,(*s->mouse_x),'@',city_text_005f8a48,8,y + 0x9a,(*c->small_image),0xffff,(*c->origin_x),
                   (*c->origin_y));
        number(c,(*s->mouse_y),'@',city_text_005f8a3c,c->ui->drawing->cursor_x + 0x6c,y + 0x9a,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        number(c,(*s->mouse_movement),'@',city_text_005f8a2c,8,y + 0xa8,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
        number(c,(*s->sidebar_x),'@',city_text_005f8a1c,c->ui->drawing->cursor_x + 0x6c,y + 0xa8,
                   (*c->small_image),0xffff,(*c->origin_x),(*c->origin_y));
        c->ui->drawing->cursor_x = 0;
        number(c,(*s->building_in_progress),'@',city_text_005f8a04,8,y + 0xb6,(*c->small_image),0xffff,
                   (*c->origin_x),(*c->origin_y));
      }
      y = y + 0x32;
      ++player;
    } while (y < 0x1bd);
  }
  return;
}



enum debugTextColor {
    DebugColorLabel = 0xffff,
    DebugColorValue = 0x3EFA,
    DebugColorWarning = 0xf800,
};

enum debugNumberPrefix {
    DebugNumberPrefixHouseModel = '(',
    DebugNumberPrefixGrid = ' ',
};

enum debugPanelLayout {
    DebugPanelWidth = 192,
    DebugPanelWidthBlocks = 12,
    DebugPanelHeightBlocks = 15,
    DebugBuildingPanelHeightBlocks = 19,
    DebugPanelStyle = 1,
    DebugPanelY = 220,
    DebugBuildingPanelY = 156,
    DebugColumnLeft = 8,
    DebugColumnRight = 100,
    DebugColumnRightWide = 108,
    DebugColumnRightFar = 110,
    DebugHouseColumn2 = 50,
    DebugHouseColumn4 = 150,
    DebugHouseColumn5 = 200,
};


static void building_drawnobuildinginfo(const PharaohDebugBuildingContext *s, int32_t y);
static void building_title(const PharaohDebugBuildingContext *s, const char *text, int32_t y, int32_t font, int32_t color);
static void building_drawgoods(const PharaohDebugBuildingContext *s, const int16_t *inventory, int32_t y);
static void building_drawentertainmentinfo(const PharaohDebugBuildingContext *s, const PharaohDebugBuilding *b, int32_t y);
static void building_drawhouseinfo(const PharaohDebugBuildingContext *s, const PharaohDebugBuilding *b, int32_t y);
static void building_drawbuildingstock(const PharaohDebugBuildingContext *s, const PharaohDebugBuilding *b, int32_t y);
static int32_t building_showsrawmaterials(int32_t type);
static void building_num(const PharaohDebugBuildingContext *s, int32_t value, const char *label, int32_t x, int32_t y, int32_t color);
static void building_housemodelvalue(const PharaohDebugBuildingContext *s, int32_t value, int32_t x, int32_t y);
static void building_label(const PharaohDebugBuildingContext *s, const char *text, int32_t x, int32_t y, int32_t color);
void pharaoh_debug_building_draw(const PharaohDebugBuildingContext *s);

static void building_drawnobuildinginfo(const PharaohDebugBuildingContext *s, int32_t y)
{
    const PharaohDebugInfoText *const c = s->text;
    building_title(s, "Not over a building", y + 120, (*c->small_image), DebugColorValue);
    building_num(s, (*s->selected_grid), " = over ptr", DebugColumnLeft, y + 140, DebugColorLabel);
    pharaoh_ui_text_draw_centered_language(c->ui,c->small_font,
        16, (*s->grid_overlay), DebugColumnLeft, y + 160, DebugPanelWidth, (*c->small_image), DebugColorLabel, (*c->origin_x), (*c->origin_y));
    building_num(s, (*s->undo_ready), " = undo_ready", DebugColumnLeft, y + 180, DebugColorLabel);
    building_num(s, (*s->undo_available), " = undo_valid", DebugColumnLeft, y + 200, DebugColorLabel);
    building_num(s, (*s->undo_count), " = undo_buildings", DebugColumnLeft, y + 220, DebugColorLabel);
    building_num(s, s->buildings[s->undo_ids[0]].state, " = alive", DebugColumnLeft, y + 240, DebugColorLabel);
    building_num(s, s->undo_buildings[0].state, " = alive", DebugColumnRight, y + 240, DebugColorLabel);
    c->ui->drawing->cursor_x = 0;
    for (int32_t direction = 0; direction < 8; direction++) {
        building_num(s, s->water_routing[(*s->selected_grid) + s->direction_deltas[direction]], ",", c->ui->drawing->cursor_x + 8, y + 260, DebugColorValue);
    }
    building_num(s, (*s->redraw), " = redraw", DebugColumnRight, y + 280, DebugColorLabel);
}

static void building_title(const PharaohDebugBuildingContext *s, const char *text, int32_t y, int32_t font, int32_t color)
{
    const PharaohDebugInfoText *const c = s->text;
    center(c, text, 0, y, DebugPanelWidth, font, color);
}

static void building_drawgoods(const PharaohDebugBuildingContext *s, const int16_t *inventory, int32_t y)
{
    const PharaohDebugInfoText *const c = s->text;
    c->ui->drawing->cursor_x = 0;
    building_num(s, inventory[0], " ,", DebugColumnLeft, y, DebugColorValue);
    building_num(s, inventory[9], " ,", c->ui->drawing->cursor_x + 8, y, DebugColorValue);
    building_num(s, inventory[11], " ,", c->ui->drawing->cursor_x + 8, y, DebugColorValue);
    building_num(s, inventory[10], " ,", c->ui->drawing->cursor_x + 8, y, DebugColorValue);
    building_num(s, inventory[8], " goods", c->ui->drawing->cursor_x + 8, y, DebugColorValue);
}

static void building_drawentertainmentinfo(const PharaohDebugBuildingContext *s, const PharaohDebugBuilding *b, int32_t y)
{
    const PharaohDebugInfoText *const c = s->text;
    building_num(s, b->data.entertainment.numShows, " shows", DebugColumnLeft, y, DebugColorValue);
    c->ui->drawing->cursor_x = 0;
    building_num(s, b->data.entertainment.showDaysLeft[0], ", ", DebugColumnRightWide, y, DebugColorValue);
    building_num(s, b->data.entertainment.showDaysLeft[1], "  ", c->ui->drawing->cursor_x + DebugColumnRightWide, y, DebugColorValue);
    building_num(s, b->data.entertainment.showDaysLeft[2], "  ", c->ui->drawing->cursor_x + DebugColumnRightWide, y, DebugColorValue);
}

static void building_drawhouseinfo(const PharaohDebugBuildingContext *s, const PharaohDebugBuilding *b, int32_t y)
{
    const PharaohDebugInfoText *const c = s->text;
    const PharaohDebugBuildingHouse *house = &b->data.house;
    int32_t servicesY = y + 192;
    c->ui->drawing->cursor_x = 0;
    building_num(s, house->juggler, "th", DebugColumnLeft, servicesY, DebugColorValue);
    c->ui->drawing->cursor_x = 0;
    building_num(s, house->musician, ",", DebugHouseColumn2, servicesY, DebugColorValue);
    c->ui->drawing->cursor_x = 0;
    building_num(s, house->dancer, " ", DebugColumnRight, servicesY, DebugColorValue);
    c->ui->drawing->cursor_x = 0;
    building_num(s, house->senet, "hi", DebugHouseColumn4, servicesY, DebugColorValue);
    c->ui->drawing->cursor_x = 0;
    building_num(s, house->zoo, "hi", DebugHouseColumn5, servicesY, DebugColorValue);
    int32_t levelsY = y + 240;
    building_num(s, house->entertainment, " en", DebugColumnLeft, levelsY, DebugColorValue);
    building_num(s, house->education, " ed", DebugHouseColumn2, levelsY, DebugColorValue);
    building_num(s, house->health, " he", DebugColumnRight, levelsY, DebugColorValue);
    building_num(s, house->numGods, " re", DebugHouseColumn4, levelsY, DebugColorValue);
    int32_t modelY = y + 252;
    const PharaohHouseModel *model = &s->models[b->subtype];
    building_housemodelvalue(s, model->entertainment, DebugColumnLeft, modelY);
    building_housemodelvalue(s, model->education, DebugHouseColumn2, modelY);
    building_housemodelvalue(s, model->religion, DebugColumnRight, modelY);
    building_housemodelvalue(s, model->health, DebugHouseColumn4, modelY);
}

static void building_drawbuildingstock(const PharaohDebugBuildingContext *s, const PharaohDebugBuilding *b, int32_t y)
{
    const PharaohDebugInfoText *const c = s->text;
    if (b->type == 90) {
        c->ui->drawing->cursor_x = 0;
        building_label(s, "aqua links", DebugColumnLeft, y, DebugColorValue);
        for (int32_t i = 0; i < (int32_t) (sizeof(b->data.waterLift.aqueductLinks) / sizeof(b->data.waterLift.aqueductLinks[0])); i++) {
            building_num(s, b->data.waterLift.aqueductLinks[i], " ", c->ui->drawing->cursor_x + 8, y, DebugColorValue);
        }
    } else if (b->type == 71 || b->type == 191) {
        c->ui->drawing->cursor_x = 0;
        building_label(s, "food", DebugColumnLeft, y, DebugColorValue);
        for (int32_t i = 0; i < 7; i++) {
            building_num(s, b->data.granary.resourceStored[i], " ", c->ui->drawing->cursor_x + 8, y, DebugColorValue);
            c->ui->drawing->cursor_x = c->ui->drawing->cursor_x - 4;
        }
    } else if (building_showsrawmaterials(b->type)) {
        c->ui->drawing->cursor_x = 0;
        building_label(s, "raw_materials", DebugColumnLeft, y, DebugColorValue);
        building_num(s, b->loadsStored, " ", c->ui->drawing->cursor_x + 8, y, DebugColorValue);
    }
}

static int32_t building_showsrawmaterials(int32_t type)
{
    if (type >= 110 && type <= 114) {
        return 1;
    }
    switch (type) {
        case 32:
        case 47:
        case 51:
        case 53:
        case 74:
        case 113:
        case 177:
        case 178:
        case 187:
        case 188:
        case 189:
        case 194:
        case 203:
        case 204:
        case 205:
        case 226:
        case 231:
        case 232:
        case 233:
            return 1;
        default:
            return 0;
    }
}

static void building_num(const PharaohDebugBuildingContext *s, int32_t value, const char *label, int32_t x, int32_t y, int32_t color)
{
    const PharaohDebugInfoText *const c = s->text;
    number(c,value, '@', label, x, y, (*c->small_image), color, (*c->origin_x), (*c->origin_y));
}

static void building_housemodelvalue(const PharaohDebugBuildingContext *s, int32_t value, int32_t x, int32_t y)
{
    const PharaohDebugInfoText *const c = s->text;
    number(c,value, DebugNumberPrefixHouseModel, ")", x, y, (*c->small_image), DebugColorValue, (*c->origin_x), (*c->origin_y));
}

static void building_label(const PharaohDebugBuildingContext *s, const char *text, int32_t x, int32_t y, int32_t color)
{
    const PharaohDebugInfoText *const c = s->text;
    plain(c, text, x, y, (*c->small_image), color);
}

void pharaoh_debug_building_draw(const PharaohDebugBuildingContext *s)
{
    const PharaohDebugInfoText *const c = s->text;
    if ((*s->enabled) != 1) {
        return;
    }
    int32_t y = ((*c->origin_y) * 2) + DebugBuildingPanelY;
    pharaoh_panel_blocks_draw(c->panel,6, 0, y, DebugPanelWidthBlocks, DebugBuildingPanelHeightBlocks, DebugPanelStyle);
    (*s->selected_grid) = pharaoh_city_view_mouse_to_grid_offset(s->city->state,s->city->projection,s->hit,*s->mouse_x,*s->mouse_y);
    int32_t id = (*s->selected_grid) == 0 ? 0 : s->building_ids[(*s->selected_grid)];
    building_title(s, "Building info (F4)", y + 6, (*c->normal_image), DebugColorLabel);
    if (id == 0) {
        building_drawnobuildinginfo(s, y);
        return;
    }
    const PharaohDebugBuilding *b = &s->buildings[id];
    building_num(s, id, " = building no", DebugColumnLeft, y + 24, b->state != 1 ? DebugColorWarning : DebugColorValue);
    building_num(s, b->type, " = type", DebugColumnLeft, y + 60, DebugColorValue);
    building_num(s, b->subtype, " = sub_type", DebugColumnRight, y + 60, DebugColorValue);
    building_drawbuildingstock(s, b, y + 72);
    building_num(s, b->housePopulation, " = pop", DebugColumnLeft, y + 84, DebugColorValue);
    building_num(s, b->loadsStored, " = goods", DebugColumnRight, y + 84, DebugColorValue);
    building_num(s, b->housesCovered, " = acc", DebugColumnLeft, y + 96, DebugColorValue);
    building_num(s, b->percentageHousesCovered, " = rel", DebugColumnRight, y + 96, DebugColorValue);
    building_num(s, b->immigrantWalkerId, " = immigrant", DebugColumnLeft, y + 108, DebugColorValue);
    if (b->walkerId2 != 0) {
        building_num(s, b->walkerId2, " = SUB agent", DebugColumnRight, y + 108, DebugColorValue);
    } else if (b->walkerId != 0) {
        building_num(s, b->walkerId, " = agent", DebugColumnRight, y + 108, DebugColorValue);
    }
    building_num(s, b->distanceFromEntry, " = rome", DebugColumnLeft, y + 120, DebugColorValue);
    building_num(s, b->roadNetworkId, " = roadnet", DebugColumnRight, y + 120, DebugColorValue);
    building_num(s, b->housePopulationRoom, " = spare room", DebugColumnLeft, y + 132, DebugColorValue);
    building_num(s, b->damageRisk, " = damage", DebugColumnRight, y + 132, DebugColorValue);
    if (b->hasWaterAccess != 0) {
        building_num(s, b->hasWaterAccess, " = water sup", DebugColumnLeft, y + 144, DebugColorValue);
    } else if (b->hasWellAccess != 0) {
        building_num(s, b->hasWellAccess, " = well sup", DebugColumnLeft, y + 144, DebugColorValue);
    } else {
        building_num(s, 0, " = no water", DebugColumnLeft, y + 144, DebugColorValue);
    }
    building_num(s, b->waterDependency, " = water dep", DebugColumnRight, y + 144, DebugColorValue);
    building_num(s, b->unreachableTicks, " = no contact with rome", DebugColumnLeft, y + 156, DebugColorValue);
    building_num(s, b->numWorkers, " = employees", DebugColumnRight, y + 168, DebugColorValue);
    building_num(s, b->houseTaxCoverage, " = tax cvr", DebugColumnLeft, y + 180, DebugColorValue);
    building_num(s, b->houseHighestPopulation, " = max pop", DebugColumnRight, y + 180, DebugColorValue);
    int32_t isHouse = b->houseSize != 0;
    if (isHouse) {
        building_drawhouseinfo(s, b, y);
    } else {
        building_drawentertainmentinfo(s, b, y + 192);
    }
    building_num(s, (*s->selected_grid), " = over ptr", DebugColumnLeft, y + 204, DebugColorLabel);
    if (isHouse) {
        building_drawgoods(s, b->data.house.inventory, y + 220);
    } else if (b->type == 70 || b->type == 190) {
        building_drawgoods(s, b->data.market.inventory, y + 220);
    }
}
