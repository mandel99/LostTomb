#include "pharaoh/monuments/mausoleum_images.h"
#include <stddef.h>

_Static_assert(offsetof(PharaohBuildingTypeRecord,origin_x)==0x68 &&
    offsetof(PharaohBuildingTypeRecord,mausoleum_orientation)==0x70 &&
    offsetof(PharaohBuildingTypeRecord,monument_worker_id)==0x5c &&
    offsetof(PharaohWalkerQueryRecord,action_state)==0x38 &&
    offsetof(PharaohWalkerQueryRecord,leading_walker_id)==0x68 &&
    offsetof(PharaohWalkerQueryRecord,roam_length)==0x44,
    "native mausoleum part and worker fields");

static PharaohMausoleumPartDescriptor describe(PharaohMausoleumImageContext *c,
    const PharaohBuildingTypeRecord *b)
{
    PharaohMausoleumPartDescriptor part;
    pharaoh_mausoleum_part_describe(c->projection,&part,b->subtype,
        *c->surface->orientation,(int32_t)b->mausoleum_orientation);
    return part;
}

void pharaoh_mausoleum_update_map(PharaohMausoleumImageContext *c,int32_t id)
{
    PharaohMapSurfaceContext *const s=c->surface;
    const PharaohMausoleumPartDescriptor part=describe(c,&s->buildings[id]);
    uint32_t mode=0;
    const uint32_t image=c->query_image(c->user_data,id,&mode);
    const int32_t size=part.size;
    const int32_t x=s->buildings[id].x,y=s->buildings[id].y;
    if((mode&1u)==0){
        pharaoh_map_place_surface_square(s,(uint16_t)id,x,y,size,size,image,8);
        return;
    }
    const uint32_t base=(uint32_t)s->map->origin+(uint32_t)y*228u+(uint32_t)x;
    *s->current_tile=base;
    for(int32_t row=0;row<size;++row)for(int32_t column=0;column<size;++column){
        const uint32_t tile=base+(uint32_t)s->footprints[row][column].grid_offset;
        const uint32_t flags=s->map->terrain[tile]|0x10000000u;
        s->building_ids[tile]=(int16_t)id;s->property_flags[tile]&=0xe0u;
        s->edge_flags[tile]|=0x40u;s->map->terrain[tile]=flags;
        if(image==0)s->image_ids[tile]=0;
        else{
            s->image_ids[tile]=image+(image==s->groups[500].first_image?
                s->random[tile]&7u:0u);
            s->map->terrain[tile]=flags&((image==s->groups[790].first_image ||
                image==s->groups[759].first_image)?0xfefed7feu:0xfefed77eu);
        }
    }
    if(image==0){
        const int32_t right=(int32_t)((uint32_t)x+(uint32_t)size-1u);
        const int32_t bottom=(int32_t)((uint32_t)y+(uint32_t)size-1u);
        pharaoh_map_refresh_trees(s,x,y,right,bottom);
        pharaoh_map_refresh_empty_region(s,x,y,right,bottom,0,1);
        pharaoh_map_refresh_meadow(s,x,y,right,bottom);
    }
}

void pharaoh_mausoleum_reposition_part(PharaohMausoleumImageContext *c,int32_t id)
{
    PharaohBuildingTypeRecord *const b=&c->surface->buildings[id];
    const PharaohMausoleumPartDescriptor part=describe(c,b);
    int32_t x=(int32_t)((uint32_t)b->origin_x+(uint32_t)part.dx);
    int32_t y=(int32_t)((uint32_t)b->origin_y+(uint32_t)part.dy);
    b->x=(int16_t)x;b->y=(int16_t)y;
    b->grid_offset=(int32_t)((uint32_t)c->surface->map->origin+
        (uint32_t)y*228u+(uint32_t)x);
    /* Native004E0C50 views the building buffer from +4, at stride264:
     * its x/y WORDs at +2/+4 are the building's +6/+8 fields. */
    const void *const work_tiles=(const uint8_t *)c->surface->buildings+4;
    (void)pharaoh_mausoleum_get_work_tile(work_tiles,id,&x,&y);
    b->road_access_x=(int16_t)x;b->road_access_y=(int16_t)y;
    int32_t worker=b->monument_worker_id;
    if(worker!=0){
        PharaohWalkerQueryRecord *w=&c->walkers[worker];
        const uint8_t type=w->type,state=w->action_state;
        if(((type==79 || type==81) && state==9) || (type==85 && state==27)){
            w->destination_x=b->road_access_x;w->destination_y=(int16_t)y;
            w->destination_grid_offset=b->grid_offset;
            goto retarget;
        }
        if((type==79 && state==10) || (type==85 && state==28) ||
                (type==81 && (state==10 || state==11))){
            c->reset_route(c->user_data,worker);
            w->x=b->x;w->y=b->y;const int32_t tile=b->grid_offset;
            w->grid_offset=tile;if(type==81)w->destination_grid_offset=tile;
            c->restart_movement(c->user_data,worker);
            if(type==81 && w->action_state==10){
                const int32_t related=c->walkers[*c->current_walker_id].leading_walker_id;
                if(related!=0){
                    w=&c->walkers[related];
                    if(w->type==85 && w->action_state==26){
                        worker=related;w->destination_x=b->road_access_x;
                        w->destination_y=b->road_access_y;
                        w->destination_grid_offset=b->grid_offset;
retarget:
                        pharaoh_walker_detach_slot_link(c->walkers,c->slot_walkers,worker);
                        w->roam_length=0;
                    }
                }
            }
        }
    }
    pharaoh_mausoleum_update_map(c,id);
}

static void reposition(void *data,int32_t id)
{pharaoh_mausoleum_reposition_part(data,id);}
void pharaoh_rotated_bind_mausoleum_image(PharaohRotatedBuildingOperations *ops,
    PharaohMausoleumImageContext *c)
{
    ops->mausoleum=reposition;ops->mausoleum_user_data=c;
}
