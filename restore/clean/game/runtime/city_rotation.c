#include "pharaoh/runtime/city_rotation.h"
#include "pharaoh/map/terrain_queries.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohWalkerQueryRecord)==388 &&
    PHARAOH_WALKER_RECORD_COUNT==2000 &&
    offsetof(PharaohWalkerQueryRecord,type)==0x0a &&
    offsetof(PharaohWalkerQueryRecord,x)==0x14 &&
    offsetof(PharaohWalkerQueryRecord,previous_x)==0x18 &&
    offsetof(PharaohWalkerQueryRecord,grid_offset)==0x20 &&
    offsetof(PharaohWalkerQueryRecord,destination_x)==0x24 &&
    offsetof(PharaohWalkerQueryRecord,source_x)==0x2c &&
    offsetof(PharaohWalkerQueryRecord,action_state)==0x38 &&
    offsetof(PharaohWalkerQueryRecord,progress_on_tile)==0x39 &&
    offsetof(PharaohWalkerQueryRecord,cross_country_x)==0x4a &&
    offsetof(PharaohWalkerQueryRecord,building_id)==0x5a,
    "native rotation walker fields");
_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
    offsetof(PharaohBuildingTypeRecord,x)==6 &&
    offsetof(PharaohBuildingTypeRecord,y)==8,
    "native rotation building coordinates");

static int32_t add32(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub32(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a-(uint32_t)b);}

/* Reuse complete CityViewManagerRotation.c, qualified against both58-instruction
 * Native bodies. Width70E464 and height70E460 are intentionally not swapped.
 * Left wraps only above6; right wraps only below0, as the original branches do. */
static void rotate_view(PharaohCityRotationContext *c,int32_t left)
{
    PharaohCityViewState *const state=c->view->state;
    const uint32_t center_index=(uint32_t)(state->viewport.width_tiles/2)+
        (uint32_t)state->camera_x+231u*((uint32_t)(state->viewport.height_tiles/2)+
        (uint32_t)state->camera_y);
    const int32_t center=c->view->projection->cells[center_index];
    int32_t orientation;
    if(left) {
        orientation=add32(*c->city_orientation,2);
        *c->city_orientation=orientation;
        if(orientation>6)*c->city_orientation=0;
    } else {
        orientation=sub32(*c->city_orientation,2);
        *c->city_orientation=orientation;
        if(orientation<0)*c->city_orientation=6;
    }
    pharaoh_city_view_calculate_lookup(c->view,*c->city_orientation);
    if(center<INT32_C(0x0fff0000)) {
        pharaoh_city_view_grid_offset_to_xy_view(c->view->projection,center,
            c->result_x,c->result_y);
        if(left) {
            state->camera_y=sub32(*c->result_y,state->viewport.height_tiles/2);
            state->camera_x=sub32(*c->result_x,state->viewport.width_tiles/2);
            orientation=*c->city_orientation;
            if(orientation==0 || orientation==4)state->camera_x=add32(state->camera_x,1);
        } else {
            state->camera_x=sub32(*c->result_x,state->viewport.width_tiles/2);
            state->camera_y=sub32(*c->result_y,state->viewport.height_tiles/2);
            orientation=*c->city_orientation;
            if(orientation==0 || orientation==4)state->camera_y=add32(state->camera_y,2);
        }
    }
}

void pharaoh_city_view_rotate_left(PharaohCityRotationContext *c)
{rotate_view(c,1);}
void pharaoh_city_view_rotate_right(PharaohCityRotationContext *c)
{rotate_view(c,0);}

/* Whole existing Unit01345 C, Native00497B90. Scan IDs1..1999 and preserve
 * every BYTE/WORD store, the global-current-walker loads and call order. */
void pharaoh_city_rotation_repair_sentries(PharaohCityRotationWalkerContext *c)
{
    PharaohWalkerQueryRecord *const walkers=c->tile_chain->walkers;
    for(int32_t id=1;id<PHARAOH_WALKER_RECORD_COUNT;++id) {
        PharaohWalkerQueryRecord *const w=&walkers[id];
        if(w->type!=42 || c->marked_tiles[w->grid_offset]==0)continue;
        const int32_t found=pharaoh_map_find_unmarked_tile_within_radius(
            c->marked_tiles,*c->origin,*c->width,*c->height,
            w->x,w->y,1,2,c->search_tile,c->result_x,c->result_y);
        if(!found) {
            w->action_state=8;
            pharaoh_walker_detach_from_tile(c->tile_chain,id);
            const PharaohBuildingTypeRecord *const b=&c->buildings[w->building_id];
            const int16_t x=b->x,y=b->y;
            w->x=x;w->source_x=x;w->y=y;w->source_y=y;
            w->grid_offset=(int32_t)((uint32_t)*c->origin+(uint32_t)(int32_t)w->x+
                (uint32_t)(int32_t)w->y*228u);
            pharaoh_walker_attach_to_tile(c->tile_chain,id);
            w->action_state=8;
            pharaoh_walker_detach_slot_link(walkers,c->slot_walkers,id);
        } else {
            pharaoh_walker_detach_slot_link(walkers,c->slot_walkers,(int32_t)*c->current_walker);
            w->progress_on_tile=0;
            pharaoh_walker_detach_from_tile(c->tile_chain,id);
            const int32_t x=*c->result_x,y=*c->result_y;
            w->x=(int16_t)x;w->previous_x=(int16_t)x;
            w->y=(int16_t)y;w->previous_y=(int16_t)y;
            w->cross_country_x=(int16_t)((uint32_t)x*15u);
            w->cross_country_y=(int16_t)((uint32_t)y*15u);
            w->grid_offset=(int32_t)((uint32_t)*c->origin+(uint32_t)x+(uint32_t)y*228u);
            pharaoh_walker_attach_to_tile(c->tile_chain,id);
            PharaohWalkerQueryRecord *const current=&walkers[*c->current_walker];
            current->action_state=11;
            current->destination_x=current->source_x;
            current->destination_y=current->source_y;
        }
    }
}

/* Existing Unit01134 sequencer; final4DDF90 is a real tail-call, not a hook. */
void pharaoh_city_rotation_rebuild(PharaohCityRotationContext *c)
{
    pharaoh_map_update_entry_exit_flags(c->surface,1);
    *c->undo_available=0;
    pharaoh_map_update_orientation_corners(c->map_leaf);
    pharaoh_map_refresh_all_tiles(c->surface);
    pharaoh_map_refresh_rotated_building_images(c->rotated_buildings);
    pharaoh_map_rotate_bridge_sprites(c->map_leaf);
    pharaoh_map_routing_initialize_walls(c->routing);
    pharaoh_city_rotation_repair_sentries(c->walker_repair);
    pharaoh_city_view_calculate_map_bounds(c->view);
}

static void finish_rotation(PharaohCityRotationContext *c)
{
    pharaoh_city_rotation_rebuild(c);
    pharaoh_city_view_check_camera_boundaries(c->view);
    pharaoh_city_warning_post(c->warning,1);
    *c->redraw_requested=1;
}

/* Whole Unit00074 C,55 Native instructions. This is a logical void UI action;
 * incidental EAX/flags and unobserved indirect consumers remain unqualified.
 * Do not collapse the two rebuilds in the back view. */
void pharaoh_city_rotate_command(PharaohCityRotationContext *c)
{
    const uint32_t parameter=*c->button_parameter;
    if(parameter==0) {
        const int32_t orientation=*c->map_orientation;
        if(orientation==0)return;
        if(orientation==2) {
            *c->bridge_turn=1;
            pharaoh_city_view_rotate_right(c);
            *c->map_orientation=*c->city_orientation;
        } else {
            *c->bridge_turn=0;
            pharaoh_city_view_rotate_left(c);
            if(orientation!=6)pharaoh_city_view_rotate_left(c);
            *c->map_orientation=*c->city_orientation;
            if(orientation!=6)pharaoh_city_rotation_rebuild(c);
        }
        finish_rotation(c);
        return;
    }
    if(parameter==2) {
        pharaoh_city_view_rotate_left(c);
        *c->bridge_turn=*c->button_parameter;
    } else if(parameter==1) {
        pharaoh_city_view_rotate_right(c);
        *c->bridge_turn=*c->button_parameter;
    } else *c->bridge_turn=parameter;
    *c->map_orientation=*c->city_orientation;
    finish_rotation(c);
}
