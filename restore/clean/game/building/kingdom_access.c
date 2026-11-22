#include "pharaoh/building/kingdom_access.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
    offsetof(PharaohBuildingTypeRecord,road_network_id)==0x14 &&
    offsetof(PharaohBuildingTypeRecord,access_failure_ticks)==0x24 &&
    offsetof(PharaohScenarioPlayerRuntime,kingdom_entry_x)==0x2778 &&
    offsetof(PharaohScenarioPlayerRuntime,kingdom_exit_tile)==0x2784 &&
    offsetof(PharaohScenarioPlayerRuntime,access_anchor_building_id)==0x4674,
    "native kingdom access overlays");

static PharaohScenarioPlayerRuntime *player(PharaohBuildingAccessContext *c)
{
    return &c->players[*c->current_player];
}

/* Complete native00430CD0; retain the native16-bit even-Y mask. */
void pharaoh_city_view_focus_tile(PharaohCityViewContext *view,int32_t tile,
    int32_t *result_x,int32_t *result_y)
{
    pharaoh_city_view_grid_offset_to_xy_view(view->projection,tile,
        result_x,result_y);
    view->state->camera_x=(int32_t)((uint32_t)*result_x-
        (uint32_t)(view->state->viewport.width_tiles/2));
    view->state->camera_y=(int32_t)(((uint32_t)*result_y-
        (uint32_t)(view->state->viewport.height_tiles/2))&0xfffeu);
    view->state->redraw_requested=1;
    pharaoh_city_view_check_camera_boundaries(view);
}

/* Reused BuildingAccessSearch C; complete native00458270 controller. */
void pharaoh_building_remove_highest_serial(PharaohBuildingAccessContext *c)
{
    const PharaohBuildingAccessOperations *const ops=c->operations;
    const int32_t count=*c->highest_id;
    int32_t chosen=0,best=0;
    for (int32_t i=1;i<=count;++i) {
        const PharaohBuildingTypeRecord *const b=&c->buildings[i];
        if ((b->state==1 || b->state==3) && b->runtime_value_016>best) {
            best=b->runtime_value_016;chosen=i;
        }
    }
    if (chosen==0) return;
    chosen=pharaoh_building_follow_link(c->buildings,chosen);
    PharaohBuildingTypeRecord *const b=&c->buildings[chosen];
    *c->event_popup=1;
    ops->post_message(ops->user_data,279,0,b->grid_offset,0,0);
    b->state=4;
    *c->removal_undo=0;
    if (b->building_type==202) ops->remove_special(ops->user_data,chosen);
    else ops->remove_footprint(ops->user_data,chosen,b->x,b->y,b->size,b->size);
    const int32_t kind=b->building_type;
    if (kind==31 || kind==30 || kind==33 || kind==209)
        ops->clear_farm_tiles(ops->user_data,chosen,kind);
    ops->explosion_cloud(ops->user_data,b->x,b->y,b->size,1);
    ops->clear_parts(ops->user_data,chosen,0);
    uint32_t x=(uint32_t)(int32_t)b->x,y=(uint32_t)(int32_t)b->y,size=b->size;
    ops->refresh_water(ops->user_data,(int32_t)(x-size),(int32_t)(y-size),
        (int32_t)(x+size),(int32_t)(y+size),1);
    x=(uint32_t)(int32_t)b->x;y=(uint32_t)(int32_t)b->y;size=b->size;
    ops->refresh_citizen(ops->user_data,(int32_t)(x-size),(int32_t)(y-size),
        (int32_t)(x+size),(int32_t)(y+size));
}

static PharaohRoadAccessMap access_map(PharaohBuildingAccessContext *c)
{
    PharaohRoadAccessMap map=*c->access;
    map.origin=c->surface->map->origin;
    map.width=c->surface->map->width;
    map.height=c->surface->map->height;
    map.preferred_regions=player(c)->largest_road_networks;
    return map;
}

static void publish_neighbor(PharaohBuildingAccessContext *c,
    PharaohBuildingTypeRecord *b,uint32_t result,int32_t late_position)
{
    const uint32_t tile=result-1u;
    const uint8_t color=c->access->region_color[tile];
    const uint16_t distance=(uint16_t)c->access->routing_distance[tile];
    b->road_network_id=color;
    if (late_position) {
        b->distance_from_entry=(int16_t)distance;
        b->road_access_x=(int16_t)*c->access->result_x;
        b->road_access_y=(int16_t)*c->access->result_y;
    } else {
        const int16_t y=(int16_t)*c->access->result_y;
        const int16_t x=(int16_t)*c->access->result_x;
        b->distance_from_entry=(int16_t)distance;
        b->road_access_x=x;b->road_access_y=y;
    }
}

static void publish_position(PharaohBuildingAccessContext *c,
    PharaohBuildingTypeRecord *b,int32_t early_distance)
{
    const uint32_t y=(uint32_t)*c->access->result_y;
    const uint32_t x=(uint32_t)*c->access->result_x;
    const uint32_t tile=(uint32_t)c->surface->map->origin+y*228u+x;
    uint16_t distance=0;
    if (early_distance) distance=(uint16_t)c->access->routing_distance[tile];
    b->road_network_id=c->access->region_color[tile];
    if (!early_distance) distance=(uint16_t)c->access->routing_distance[tile];
    b->distance_from_entry=(int16_t)distance;
    b->road_access_x=(int16_t)x;b->road_access_y=(int16_t)y;
}

static int32_t position_index(int32_t kind)
{
    switch (kind) {
    case 213:return PHARAOH_ACCESS_SUN_TEMPLE;
    case 225:return PHARAOH_ACCESS_LIBRARY;
    case 230:return PHARAOH_ACCESS_ENTRANCE;
    case 227:return PHARAOH_ACCESS_CAESAREUM;
    case 228:return PHARAOH_ACCESS_LIGHTHOUSE;
    case 222:return PHARAOH_ACCESS_MAUSOLEUM;
    case 229:return PHARAOH_ACCESS_SMALL_TOMB;
    case 234:return PHARAOH_ACCESS_MEDIUM_TOMB;
    case 235:return PHARAOH_ACCESS_LARGE_TOMB;
    case 236:return PHARAOH_ACCESS_GRAND_TOMB;
    default:return -1;
    }
}

static void update_house(PharaohBuildingAccessContext *c,
    PharaohBuildingTypeRecord *b,uint32_t *pending)
{
    const PharaohBuildingAccessOperations *const ops=c->operations;
    PharaohRoadAccessMap map=access_map(c);
    if (!pharaoh_map_road_exists_within_distance(&map,b->x,b->y,b->size,2)) {
        b->distance_from_entry=0;
        b->access_failure_ticks=(uint16_t)(b->access_failure_ticks+1u);
        if ((int16_t)b->access_failure_ticks>4) {
            if (b->house_population!=0) {
                ops->displace_house(ops->user_data,*c->current_player,
                    b->x,b->y,b->house_population);
                b->house_population=0;b->access_failure_ticks=0;
            }
            b->state=2;
        }
    } else {
        const uint32_t tile=(uint32_t)*map.last_tile;
        if (c->access->routing_distance[tile]==0) {
            b->distance_from_entry=0;
            const uint8_t empty=b->building_type==10 && b->house_population==0;
            uint16_t timer=b->access_failure_ticks;
            if (!timer && (empty || !*pending)) *pending=(uint32_t)b->grid_offset;
            timer=(uint16_t)(timer+1u);b->access_failure_ticks=timer;
            if ((int16_t)timer>8) {
                b->access_failure_ticks=0;
                if (empty) b->state=2;
            }
        } else {
            b->distance_from_entry=(int16_t)c->access->routing_distance[tile];
            b->access_failure_ticks=0;
        }
    }
}

/* Reused BuildingAccess C; complete native00457AF0. Child operations above
 * remain mandatory, explicit native boundaries rather than fallback actions. */
void pharaoh_building_update_kingdom_access(PharaohBuildingAccessContext *c)
{
    const PharaohBuildingAccessOperations *const ops=c->operations;
    PharaohRoutingSearchState *const route=c->repair->routing;
    route->grid_start=c->surface->map->origin;
    (void)pharaoh_map_routing_citizen_access(route,player(c)->kingdom_entry_x,
        player(c)->kingdom_entry_y,-2,-2);
    uint32_t pending=0;
    for (int32_t id=1;id<=*c->highest_id;++id) {
        PharaohBuildingTypeRecord *const b=&c->buildings[id];
        if (b->state!=1) continue;
        if (b->house_size!=0) {update_house(c,b,&pending);continue;}
        const int32_t kind=b->building_type;
        PharaohRoadAccessMap map=access_map(c);
        if (kind==72 || kind==193) {
            if (!player(c)->access_anchor_building_id)
                player(c)->access_anchor_building_id=id;
            b->distance_from_entry=0;
            const uint32_t result=pharaoh_building_find_access(&map,b->x,b->y,3);
            if (result) publish_neighbor(c,b,result,1);
            continue;
        }
        b->distance_from_entry=0;
        if (kind==210) {
            /* Possible original bug:487D00 returns a zero-based tile, but the
             * common publication path still reads result-1. Retain it. */
            const uint32_t result=(uint32_t)pharaoh_map_sphinx_access_tile(
                c->surface->map->origin,b->x,b->y,map.result_x,map.result_y);
            if (result) publish_neighbor(c,b,result,0);
        } else {
            const int32_t index=position_index(kind);
            if (index>=0) {
                const PharaohBuildingAccessPosition *const position=&ops->monument[index];
                if (position->find(position->user_data,id,map.result_x,map.result_y)) {
                    if (kind==213 || kind==222) {
                        const int16_t x=(int16_t)*map.result_x;
                        const int16_t y=(int16_t)*map.result_y;
                        b->road_access_x=x;b->road_access_y=y;
                    } else publish_position(c,b,kind==225 || kind==229 ||
                        kind==234 || kind==235 || kind==236);
                }
            } else if ((kind>=65 && kind<=69) || (kind>=156 && kind<=160)) {
                const uint32_t result=pharaoh_building_find_wide_access(&map,b->x,b->y,3);
                if (result) publish_neighbor(c,b,result,0);
            } else if (kind==73) {
                const int32_t linked=pharaoh_building_follow_link(c->buildings,id);
                const PharaohBuildingTypeRecord *const other=&c->buildings[linked];
                b->road_network_id=other->road_network_id;
                b->distance_from_entry=other->distance_from_entry;
                b->road_access_x=other->road_access_x;b->road_access_y=other->road_access_y;
            } else if (kind==183) {
                uint32_t tile;
                if (pharaoh_building_special_entry(c->pyramid,id,&tile,
                        map.result_x,map.result_y)) publish_neighbor(c,b,tile+1u,1);
            } else {
                const uint32_t result=pharaoh_building_find_access(&map,b->x,b->y,b->size);
                if (result) publish_neighbor(c,b,result,0);
            }
        }
    }
    if (c->access->routing_distance[player(c)->kingdom_exit_tile]!=0) {
        if (pending) {
            ops->post_problem(ops->user_data,63,0,0);
            ops->post_problem(ops->user_data,64,0,0);
            pharaoh_city_view_focus_tile(c->view,(int32_t)pending,
                c->access->result_x,c->access->result_y);
        }
        return;
    }
    for (uint32_t attempt=0;attempt<15;++attempt) {
        route->grid_start=c->surface->map->origin;
        (void)pharaoh_map_routing_clear_first_blocker(c->repair,
            player(c)->kingdom_entry_x,player(c)->kingdom_entry_y,-2,-2);
        route->grid_start=c->surface->map->origin;
        (void)pharaoh_map_routing_clear_first_blocker(c->repair,
            player(c)->kingdom_exit_x,player(c)->kingdom_exit_y,-2,-2);
        pharaoh_map_routing_initialize_citizen(c->routes);
        pharaoh_map_routing_initialize_noncitizen(c->routes);
        pharaoh_map_routing_initialize_walls(c->routes);
        pharaoh_map_refresh_canals(c->surface);
        pharaoh_map_refresh_channel_region(c->surface,0,0,
            c->surface->map->width-1,c->surface->map->height-1,0);
        pharaoh_map_refresh_empty_region(c->surface,0,0,
            c->surface->map->width-1,c->surface->map->height-1,0,0);
        pharaoh_map_refresh_meadow(c->surface,0,0,
            c->surface->map->width-1,c->surface->map->height-1);
        pharaoh_map_refresh_gardens(c->surface);
        route->grid_start=c->surface->map->origin;
        (void)pharaoh_map_routing_citizen_access(route,player(c)->kingdom_entry_x,
            player(c)->kingdom_entry_y,-2,-2);
        if (c->access->routing_distance[player(c)->kingdom_exit_tile]!=0) {
            *c->event_popup=1;
            ops->post_message(ops->user_data,315,0,0,0,0);
            *c->removal_undo=0;
            return;
        }
    }
    pharaoh_building_remove_highest_serial(c);
}

static void update_access(void *context)
{
    pharaoh_building_update_kingdom_access(context);
}

void pharaoh_saved_game_bind_kingdom_access_step(
    PharaohSavedGameInitializeOperations *ops,PharaohBuildingAccessContext *c)
{
    ops->step[PHARAOH_INIT_KINGDOM_ACCESS]=
        (PharaohSavedGameStepBinding){c,update_access};
}
