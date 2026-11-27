#include "pharaoh/map/construction_connections.h"
#include "pharaoh/building/construction_commit_support.h"
#include "pharaoh/core/memory_strings.h"

static int32_t add32(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static uint32_t distance32(int32_t a,int32_t b)
{return a>b?(uint32_t)a-(uint32_t)b:(uint32_t)b-(uint32_t)a;}

void pharaoh_construction_clear_route_footprint(
    const PharaohConstructionConnectionContext *c,uint32_t mode,
    int32_t x,int32_t y,int32_t width,int32_t height)
{
    PharaohMapSurfaceContext *s=c->footprint->surface;
    if(x<0 || y<0)return;
    if(add32(x,add32(width,-1))>=s->map->width)return;
    if(add32(y,add32(height,-1))>=s->map->height)return;
    const uint32_t cell=(uint32_t)s->map->origin+(uint32_t)y*228u+(uint32_t)x;
    *s->current_tile=cell;
    for(int32_t row=0;row<height;++row)
        for(int32_t col=0;col<width;++col)
            if(mode==0)c->routes->distances[cell+c->footprints[row][col].grid_offset]=0;
}

static int32_t endpoint_status(const PharaohConstructionConnectionContext *c,
    int32_t x,int32_t y)
{
    PharaohMapSurfaceContext *s=c->footprint->surface;
    const uint32_t tile=(uint32_t)s->map->origin+(uint32_t)y*228u+(uint32_t)x-229u;
    const int32_t offsets[4]={(int32_t)c->footprints[0][0].grid_offset,
        (int32_t)c->footprints[0][1].grid_offset,
        (int32_t)c->footprints[1][0].grid_offset,
        (int32_t)c->footprints[1][1].grid_offset};
    if(pharaoh_building_is_water_lift_footprint(s->building_ids,s->buildings,
        offsets,(int32_t)tile))return 2;
    return pharaoh_map_empty_rectangle_fits(s,add32(x,-1),add32(y,-1),
        3,3,0xffff,0,1)?1:-1;
}

uint32_t pharaoh_construction_plan_connection(
    const PharaohConstructionConnectionContext *c,uint32_t preview,
    int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
    PharaohMapSurfaceContext *s=c->footprint->surface;
    const uint32_t base_cost=(uint32_t)c->models[8].cost;
    *c->preview_cost=0;*c->start_status=0;*c->end_status=0;
    pharaoh_memory_copy_forward_words64(c->backup->saved_terrain,s->map->terrain,0x32c40);
    pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[0],c->cleared,0xcb10);
    pharaoh_map_restore_free_tile_images(s,c->backup->saved_images);
    *c->delta_y=distance32(y1,y2);*c->delta_x=distance32(x1,x2);
    int32_t distance=(int32_t)*c->delta_x;
    if(distance<=(int32_t)*c->delta_y)distance=(int32_t)*c->delta_y;
    if(preview==0 || *c->drag_started!=0) {
        if(distance>0)*c->start_status=endpoint_status(c,x1,y1);
    } else distance=0;
    *c->end_status=endpoint_status(c,x2,y2);
    if(*c->start_status==-1 || *c->end_status==-1 ||
       (*c->start_status==1 && *c->end_status==1 && distance<=2))return 0;
    if(distance==0) {
        *c->preview_cost=(uint32_t)c->models[90].cost;
        return 1;
    }
    if(!pharaoh_map_construction_distances(c->routes,x1,y1,1))return 0;
    if(*c->start_status!=0) {
        pharaoh_construction_clear_route_footprint(c,0,add32(x1,-1),add32(y1,-1),3,3);
        pharaoh_construction_mark_footprint(c->footprint,3,add32(x1,-1),add32(y1,-1),UINT32_MAX,1,1);
    }
    if(*c->end_status!=0) {
        pharaoh_construction_clear_route_footprint(c,0,add32(x2,-1),add32(y2,-1),3,3);
        pharaoh_construction_mark_footprint(c->footprint,3,add32(x2,-1),add32(y2,-1),UINT32_MAX,1,1);
    }
    int32_t best=10000,start=0,end=0;
    for(int32_t a=0;a<4;++a)for(int32_t b=0;b<4;++b) {
        const uint32_t found=pharaoh_map_place_routed_building(c->walk,
            add32(x1,c->endpoints[a][0]),add32(y1,c->endpoints[a][1]),
            add32(x2,c->endpoints[b][0]),add32(y2,c->endpoints[b][1]),2);
        const int32_t count=(int32_t)*c->walk->tiles->placed_count;
        if(found && count!=0 && count<best){best=count;start=a;end=b;}
    }
    if(best==10000)return 0;
    (void)pharaoh_map_place_routed_building(c->walk,
        add32(x1,c->endpoints[start][0]),add32(y1,c->endpoints[start][1]),
        add32(x2,c->endpoints[end][0]),add32(y2,c->endpoints[end][1]),1);
    if(*c->start_status==1)*c->preview_cost+=(uint32_t)c->models[90].cost;
    if(*c->end_status==1)*c->preview_cost+=(uint32_t)c->models[90].cost;
    if(best!=0)*c->preview_cost+=(uint32_t)best*base_cost;
    return 1;
}

uint32_t pharaoh_construction_ferry_route_is_valid(
    const PharaohFerryRouteQueryContext *c,int32_t x1,int32_t y1,
    int32_t x2,int32_t y2,uint32_t layout,int32_t orientation,
    int32_t wanted_orientation,uint32_t *path,int32_t *count)
{
    PharaohFerryMapContext *m=c->map;
    int32_t x=x1,y=y1;
    if(*m->orientation!=0) {
        if(*m->orientation==6)y=add32(y,-1);
        else {x=add32(x,-1);if(*m->orientation==4)y=add32(y,-1);}
    }
    if(!pharaoh_map_water_tile_next_to(m,x,y,layout))return 0;
    const int32_t first_x=*m->result_x,first_y=*m->result_y;
    x=x2;y=y2;
    if(*c->ferry_state==0 && *m->orientation!=0) {
        if(*m->orientation==6)y=add32(y,-1);
        else {x=add32(x,-1);if(*m->orientation==4)y=add32(y,-1);}
    }
    if(!pharaoh_map_water_tile_next_to(m,x,y,layout))return 0;
    const int32_t last_x=*m->result_x,last_y=*m->result_y;
    (void)pharaoh_map_ferry_water_distances(m,first_x,first_y,last_x,last_y);
    const int32_t length=pharaoh_map_water_path_4dir(m,1001,first_x,first_y,last_x,last_y,0,path);
    *count=length;
    const int32_t limit=(int32_t)((uint32_t)*c->radius*2u);
    uint32_t matched=0;
    int32_t width=0,forward=0,backward=0;
    while(forward<=limit) {
        for(int32_t a=-width,b=width;a<=width;++a,--b) {
            int32_t dx=a,dy=backward;
            if(orientation!=0) {
                if(orientation==2){dx=forward;dy=b;}
                else if(orientation==4){dx=b;dy=forward;}
                else {dx=backward;dy=a;}
            }
            if(add32(dx,x1)==x2 && add32(dy,y1)==y2)matched=1;
        }
        if(++width>5)width=5;
        forward=add32(forward,1);backward=add32(backward,-1);
    }
    return orientation==wanted_orientation && length!=0 && matched;
}
