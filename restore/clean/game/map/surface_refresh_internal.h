#ifndef PHARAOH_SURFACE_REFRESH_INTERNAL_H
#define PHARAOH_SURFACE_REFRESH_INTERNAL_H
#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/map/surface_queries.h"
#include "pharaoh/runtime/platform_measure_speed.h"
static inline int32_t surface_rectangle_fits(PharaohMapSurfaceContext *c,int32_t x,int32_t y,
    int32_t width,int32_t height,uint32_t flags,uint32_t image)
{
    const PharaohMapTileArea area={c->map,c->image_ids,c->footprints,c->current_tile};
    return pharaoh_map_terrain_rectangle_fits(&area,x,y,width,height,flags,image);
}
static inline int32_t surface_rock_variant(PharaohMapSurfaceContext *c,uint32_t tile)
{
    PharaohMapRockVariantContext rock={c->adjacent,c->current_tile,c->map->terrain,
        c->building_ids,c->buildings,c->random,*c->orientation};
    return pharaoh_map_royal_tomb_rock_variant(&rock,tile);
}
static inline int32_t surface_terrain_near(PharaohMapSurfaceContext *c,int32_t x,int32_t y,
    int32_t size,int32_t radius,uint32_t mask)
{
    PharaohMapAreaCursor cursor={*c->area_x,*c->area_y,0};
    const int32_t result=pharaoh_map_terrain_exists_in_area_with(c->map,x,y,size,radius,mask,&cursor);
    *c->area_x=cursor.x;*c->area_y=cursor.y;
    return result;
}
static inline int32_t surface_match(PharaohMapSurfaceContext *c,PharaohMapImageContextRecord *records,
    int32_t count,int32_t animation,uint8_t reverse)
{
    const PharaohMapImageContext image={c->adjacent,*c->orientation,c->selection};
    return pharaoh_map_image_context_match(&image,records,count,animation,reverse);
}

static inline int32_t surface_grouped_match(PharaohMapSurfaceContext *c,
    PharaohMapImageContextRecord *records,int32_t count)
{
    const PharaohMapImageContext image={c->adjacent,*c->orientation,c->selection};
    return pharaoh_map_image_context_match_grouped(&image,records,count);
}
static inline int32_t surface_deepwater_has_two_run(const uint32_t adjacent[8])
{
    uint32_t index=0,limit=16;int32_t run=-1;
    do{
        if(run>=8)break;
        if(adjacent[index&7u]){if(run>=0)++run;}
        else{
            if(run<0)limit=index+10u;
            else if(run==2)return 1;
            run=0;
        }
        ++index;
    }while(index<limit);
    return 0;
}

#endif
