#include "pharaoh/building/demolition.h"
#include "pharaoh/building/construction_commit_support.h"
#include "pharaoh/building/venue_images.h"
#include "pharaoh/core/memory_strings.h"
#include "pharaoh/map/venue_tiles.h"
#include "pharaoh/map/construction_region_images.h"

_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
    offsetof(PharaohBuildingTypeRecord,house_size)==5 &&
    offsetof(PharaohBuildingTypeRecord,house_population)==0x1c &&
    offsetof(PharaohBuildingTypeRecord,runtime_value_038)==0x38 &&
    offsetof(PharaohBuildingTypeRecord,next_part_building_id)==0x3a &&
    offsetof(PharaohBuildingTypeRecord,runtime_value_c1)==0xc1 &&
    offsetof(PharaohBuildingTypeRecord,runtime_value_ca)==0xca &&
    offsetof(PharaohBuildingTypeRecord,repair_queue_id)==0xcb,
    "native demolition building fields");

static int needs_class_confirmation(int32_t kind)
{
    switch(kind) {
    case 65:case 66:case 67:case 68:case 69:case 97:
    case 156:case 157:case 158:case 159:case 160:return 1;
    default:return 0;
    }
}

static int preserved_kind(int32_t kind)
{
    switch(kind) {
    case 99:case 88:case 89:case 93:case 218:case 219:case 220:case 215:return 1;
    default:return 0;
    }
}

static void mark_chain(const PharaohDemolitionContext *c,int32_t node,int next)
{
    PharaohMapSurfaceContext *s=c->surface;
    for(uint32_t i=0;i<525;++i) {
        node=next?s->buildings[node].next_part_building_id:
            s->buildings[node].runtime_value_038;
        if(node<=0)break;
        (void)pharaoh_map_building_undo_capture(s->undo,node);
        s->buildings[node].state=6;
        s->buildings[node].render_tint_bf=1;
    }
}

/* Complete474700; whole ConstructionController algorithm reused. */
void pharaoh_construction_demolish(const PharaohDemolitionContext *c,
    int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
    PharaohMapSurfaceContext *s=c->surface;
    int32_t left=x1<x2?x1:x2,right=x1<x2?x2:x1;
    int32_t top=y1<y2?y1:y2,bottom=y1<y2?y2:y1,kind;
    uint32_t radius=0;
    *c->placed_count=0;
    pharaoh_map_undo_release_reservations(s->undo,s->buildings);
    pharaoh_memory_copy_forward_words64(c->backup->saved_terrain,
        s->map->terrain,0x32c40);
    pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[0],
        c->cleared,0xcb10);
    pharaoh_map_restore_free_tile_images(s,c->backup->saved_images);
    if(left<0)left=0;
    if(right>=s->map->width)right=(int32_t)((uint32_t)s->map->width-1u);
    if(top<0)top=0;
    if(bottom>=s->map->height)bottom=(int32_t)((uint32_t)s->map->height-1u);
    uint32_t width=(uint32_t)right-(uint32_t)left+1u;
    uint32_t height=(uint32_t)bottom-(uint32_t)top+1u;
    uint32_t cell=(uint32_t)s->map->origin+(uint32_t)top*228u+(uint32_t)left;
    *s->current_tile=cell;*s->scan_y=top;
    if(top<=bottom)do {
        *s->scan_x=left;
        if(left<=right)do {
            const uint32_t flags=s->map->terrain[cell],masked=flags&0xeefed77fu;
            int32_t id;
            if(masked&0x02040202u)goto next_cell;
            id=s->building_ids[cell];
            if((masked&8u)||(masked&0x40000000u))goto building;
            if(id&&(flags&0x10000000u)) {
                kind=s->buildings[id].building_type;
                if(kind==225||kind==213)goto building;
            }
            if(masked&0x100u) {
                const uint32_t count=*c->placed_count;
                s->map->terrain[cell]=flags&0x03052e80u;c->cleared[cell]=0;
                const uint8_t north=c->cleared[cell-228u];
                *c->placed_count=count+1u;
                if(north==5)c->cleared[cell-228u]=1;
                if(c->cleared[cell+1u]==6)c->cleared[cell+1u]=2;
                if(c->cleared[cell+228u]==5)c->cleared[cell+228u]=3;
                if(c->cleared[cell-1u]==6)c->cleared[cell-1u]=4;
            } else if(masked&4u) {
                if(!preview) {
                    if((int32_t)pharaoh_map_bridge_count_occupants(c->bridge,cell)>=1) {
                        pharaoh_city_warning_post(c->warnings,55);goto reload_next;
                    }
                    cell=*s->current_tile;
                }
                if(c->state->line==1) {
                    pharaoh_map_bridge_mark_or_remove(c->bridge,cell,(uint32_t)preview);
                    ++*c->placed_count;cell=*s->current_tile;
                }
            } else if(masked) {
                uint8_t edge=s->edge_flags[cell];
                s->map->terrain[cell]=flags&0x03052e80u;
                const uint8_t properties=s->property_flags[cell]&0xf0u;
                edge|=0x40u;s->property_flags[cell]=properties;s->edge_flags[cell]=edge;
                if(!preview) {s->property_flags[cell]=properties&0x70u;s->edge_flags[cell]=edge|0x40u;}
                ++*c->placed_count;
            }
            goto processed;
building:
            if(!id)goto processed;
            kind=s->buildings[id].building_type;
            if(preserved_kind(kind)||s->buildings[id].state==6)goto next_cell;
            if((kind==57||kind==54)&&!preview&&c->state->pair!=1)goto next_cell;
            if((uint8_t)pharaoh_building_type_is_monument(kind)&&!preview&&c->state->sensitive!=1)
                goto reload_next;
            kind=s->buildings[id].building_type;
            if(needs_class_confirmation(kind)&&!preview&&c->state->classes!=1)goto reload_next;
            if((kind==57||kind==54)&&!preview&&c->state->pair==1)*s->undo->available=0;
            if(kind==183) {
                pharaoh_pyramid_clear_complex_temple(c->pyramid,id);
                pharaoh_pyramid_update_auxiliary_reservation(c->pyramid,id,1);
            }
            uint32_t value=s->buildings[id].runtime_value_c1;
            if(value)c->native_records_7900c0[value][0]=0;
            value=s->buildings[id].repair_queue_id;
            if(value)c->native_records_9783a4[value][0]=0;
            value=s->buildings[id].runtime_value_ca;
            if(value)c->ferry_queues[value].queue.state=0;
            if(s->buildings[id].house_size&&s->buildings[id].house_population&&!preview) {
                const int32_t x=s->buildings[id].x,people=s->buildings[id].house_population;
                const int32_t y=s->buildings[id].y,owner=(int32_t)*c->current_player;
                pharaoh_city_spawn_displaced_population(c->displaced,owner,x,y,people);
                s->buildings[id].house_population=0;
            }
            kind=s->buildings[id].building_type;
            if(kind==108||kind==195||kind==115)s->buildings[id].runtime_value_a6=0;
            if(s->buildings[id].state!=6) {
                ++*c->placed_count;(void)pharaoh_map_building_undo_capture(s->undo,id);
            }
            s->buildings[id].state=6;
            pharaoh_sound_effect_request_play(c->sound_state,c->sound_device,2);
            s->buildings[id].render_tint_bf=1;
            mark_chain(c,id,0);mark_chain(c,id,1);cell=*s->current_tile;
processed:
            if(!preview&&(masked&0x148u))radius=5;
            goto next_cell;
reload_next:
            cell=*s->current_tile;
next_cell:
            *s->scan_x=(int32_t)((uint32_t)*s->scan_x+1u);++cell;*s->current_tile=cell;
        }while(*s->scan_x<=right);
        *s->scan_y=(int32_t)((uint32_t)*s->scan_y+1u);
        cell+=228u-width;*s->current_tile=cell;
    }while(*s->scan_y<=bottom);
    const uint32_t span=((int32_t)width>(int32_t)height?width:height)+2u;
    pharaoh_map_refresh_empty_region(s,left,top,right,bottom,0,1);
    if(radius)pharaoh_map_refresh_empty_region(s,
        (int32_t)((uint32_t)left-radius),(int32_t)((uint32_t)top-radius),
        (int32_t)((uint32_t)right+radius),(int32_t)((uint32_t)bottom+radius),1,0);
    pharaoh_map_refresh_meadow(s,left,top,right,bottom);
    pharaoh_map_refresh_marshland(s,left,top,right,bottom);
    pharaoh_map_refresh_floodplain(s,left,top,right,bottom);
    pharaoh_map_refresh_channel_region(s,left,top,right,bottom,0);
    pharaoh_map_refresh_road_square(s,left,top,(int32_t)span);
    pharaoh_map_refresh_rubble(s,left,top,right,bottom);
    pharaoh_map_refresh_gardens(s);
    height=(uint32_t)s->map->height-1u;width=(uint32_t)s->map->width-1u;
    pharaoh_map_refresh_paved_region(s,0,0,(int32_t)width,(int32_t)height,1);
    for(int32_t index=1;index<4000;++index) {
        const uint32_t state=s->buildings[index].state;
        if(!state||state==6)continue;
        kind=s->buildings[index].building_type;
        if(kind!=31&&kind!=30&&kind!=33&&kind!=209)continue;
        if(kind==30) {
            const int32_t linked=s->buildings[index].next_part_building_id;
            if(s->buildings[linked].building_type==30)
                pharaoh_map_publish_venue_adjacent_images(s,index,
                    s->buildings[index].x,s->buildings[index].y);
        }
        if(pharaoh_building_follow_link(s->buildings,index)==index&&
                s->buildings[index].venue_has_plaza) {
            PharaohVenueTileContext venue={0};venue.cleared=c->cleared;
            pharaoh_map_bind_venue_tiles(&venue,s);
            pharaoh_map_add_venue_plaza_tiles(&venue,
                s->buildings[index].venue_grid_offset,kind,(uint16_t)index);
        }
    }
    pharaoh_map_refresh_wall_region(s,left,top,(int32_t)span);
    pharaoh_map_refresh_water(s,left,top,right,bottom,0);
    if(!preview) {
        left=(int32_t)((uint32_t)left-1u);top=(int32_t)((uint32_t)top-1u);
        right=(int32_t)((uint32_t)right+1u);bottom=(int32_t)((uint32_t)bottom+1u);
        pharaoh_map_update_channel_images(s,left,top,right,bottom);
        pharaoh_map_refresh_road_region(s,left,top,right,bottom);
        pharaoh_map_routing_initialize_water(c->routing);
        pharaoh_map_routing_initialize_citizen(c->routing);
        pharaoh_map_routing_initialize_noncitizen(c->routing);
        pharaoh_map_routing_initialize_walls(c->routing);
    }
}

static void confirm(const PharaohDemolitionContext *c,uint32_t *decision)
{
    const uint8_t answer=c->popup->popup_flag;
    const int32_t y2=c->state->y2,x2=c->state->x2,x1=c->state->x1;
    *decision=answer==1?1u:UINT32_MAX;
    const int32_t y1=c->state->y1;
    pharaoh_construction_demolish(c,0,x1,y1,x2,y2);
}

/* Complete4021D0/2210/2250/2290; the real stored answer controls each DWORD. */
void pharaoh_demolition_confirm_monument(const PharaohDemolitionContext *c)
{confirm(c,&c->state->sensitive);}
void pharaoh_demolition_confirm_class(const PharaohDemolitionContext *c)
{confirm(c,&c->state->classes);}
void pharaoh_demolition_confirm_pair(const PharaohDemolitionContext *c)
{confirm(c,&c->state->pair);}
void pharaoh_demolition_confirm_bridge(const PharaohDemolitionContext *c)
{confirm(c,&c->state->line);}

static void monument_response(const void *c){pharaoh_demolition_confirm_monument(c);}
static void class_response(const void *c){pharaoh_demolition_confirm_class(c);}
static void pair_response(const void *c){pharaoh_demolition_confirm_pair(c);}
static void bridge_response(const void *c){pharaoh_demolition_confirm_bridge(c);}

/* Complete4743F0; original coordinates are saved after the normalized scan. */
void pharaoh_construction_request_demolition(const PharaohDemolitionContext *c,
    int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
    PharaohMapSurfaceContext *s=c->surface;
    int32_t left=x1<x2?x1:x2,right=x1<x2?x2:x1;
    int32_t top=y1<y2?y1:y2,bottom=y1<y2?y2:y1;
    int sensitive=0,classes=0,pair=0,line=0;
    c->state->pair=0;c->state->line=0;c->state->sensitive=0;c->state->classes=0;
    if(preview) {pharaoh_construction_demolish(c,preview,x1,y1,x2,y2);return;}
    if(left<0)left=0;
    if(right>=s->map->width)right=(int32_t)((uint32_t)s->map->width-1u);
    if(top<0)top=0;
    if(bottom>=s->map->height)bottom=(int32_t)((uint32_t)s->map->height-1u);
    const uint32_t width=(uint32_t)right-(uint32_t)left+1u;
    uint32_t cell=(uint32_t)s->map->origin+(uint32_t)top*228u+(uint32_t)left;
    *s->scan_y=top;*s->current_tile=cell;
    if(top<=bottom)do {
        *s->scan_x=left;
        if(left<=right)do {
            const int32_t id=s->building_ids[cell];
            const uint32_t marker=c->bridge->pieces[cell],water=s->map->terrain[cell]&4u;
            if(id) {
                const int32_t kind=s->buildings[id].building_type;
                if(kind==57||kind==54)pair=1;
            }
            if(marker&&water&&!id)line=1;
            if((uint8_t)pharaoh_building_type_is_monument(s->buildings[id].building_type))sensitive=1;
            if(needs_class_confirmation(s->buildings[id].building_type))classes=1;
            *s->scan_x=(int32_t)((uint32_t)*s->scan_x+1u);++cell;*s->current_tile=cell;
        }while(*s->scan_x<=right);
        *s->scan_y=(int32_t)((uint32_t)*s->scan_y+1u);
        cell+=228u-width;*s->current_tile=cell;
    }while(*s->scan_y<=bottom);
    c->state->x1=x1;c->state->y1=y1;c->state->x2=x2;c->state->y2=y2;
    if(sensitive)pharaoh_popup_window_open_context(c->popup,104,monument_response,c,2);
    else if(classes)pharaoh_popup_window_open_context(c->popup,106,class_response,c,2);
    else if(pair)pharaoh_popup_window_open_context(c->popup,16,pair_response,c,2);
    else if(line)pharaoh_popup_window_open_context(c->popup,20,bridge_response,c,2);
    else pharaoh_construction_demolish(c,0,x1,y1,x2,y2);
}
