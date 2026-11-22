#include "surface_refresh_internal.h"

/* Native004746D0. Width is the native third argument even for rectangles. */
void pharaoh_map_refresh_surrounding_empty(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t size)
{
    pharaoh_map_refresh_empty_region(c,(int32_t)((uint32_t)x-5u),(int32_t)((uint32_t)y-5u),
        (int32_t)((uint32_t)x+5u+(uint32_t)size),(int32_t)((uint32_t)y+5u+(uint32_t)size),1,0);
}

/* Native0047BF50: common image ID, native footprint edge metadata and
 * orientation-dependent draw anchor, followed by the real empty-land pass. */
void pharaoh_map_place_surface_square(PharaohMapSurfaceContext *c,uint16_t building,
    int32_t x,int32_t y,int32_t width,int32_t height,uint32_t image,uint32_t flags)
{
    const uint32_t mode=*c->building_orientation;
    uint32_t corner_x=0,corner_y=0;
    if(!mode)corner_y=(uint32_t)height-1u;
    else if(mode==2){}
    else if(mode==4)corner_x=(uint32_t)width-1u;
    else{corner_x=(uint32_t)width-1u;corner_y=(uint32_t)height-1u;}
    if(x<0 || y<0 || (int32_t)((uint32_t)x+(uint32_t)width-1u)>=c->map->width ||
        (int32_t)((uint32_t)y+(uint32_t)height-1u)>=c->map->height){
        *c->placement_failed=1;return;
    }
    const uint32_t base=(uint32_t)c->map->origin+(uint32_t)y*228u+(uint32_t)x;
    *c->current_tile=base;
    for(int32_t row=0;row<height;++row)for(int32_t col=0;col<width;++col){
        const PharaohMapTileAreaOffset *point=&c->footprints[row][col];
        const uint32_t tile=base+point->grid_offset;
        c->map->terrain[tile]=(c->map->terrain[tile]&0x03052e80u)|flags;
        c->building_ids[tile]=(int16_t)building;
        c->property_flags[tile]&=0xe0u;
        if((uint32_t)width>=2u && (uint32_t)width<=6u)
            c->property_flags[tile]|=(uint8_t)((uint32_t)width-1u);
        c->image_ids[tile]=image;c->edge_flags[tile]=(uint8_t)point->reserved;
        if((uint32_t)col==corner_x && (uint32_t)row==corner_y)c->edge_flags[tile]|=0x40u;
    }
    pharaoh_map_refresh_surrounding_empty(c,x,y,width);
}
