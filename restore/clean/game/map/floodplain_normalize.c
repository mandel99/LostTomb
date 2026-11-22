#include "surface_refresh_internal.h"
#include "pharaoh/map/moisture.h"
#include "pharaoh/core/static_objects.h"

uint8_t pharaoh_map_extend_floodplain(PharaohMapSurfaceContext *c,int32_t xIn,int32_t yIn)
{
    uint32_t x=(uint32_t)xIn,y=(uint32_t)yIn,left=x-1u,right,width,top=y-1u,bottom,step,cell,row,col,flags,changed=0;
    width=(c->map->width);right=width;if((int32_t)left<0)left=0;if((int32_t)right>=(int32_t)(x+1u))right=x+1u;
    bottom=(c->map->height);if((int32_t)top<0)top=0;if((int32_t)bottom>=(int32_t)(y+1u))bottom=y+1u;
    step=(*c->border_size)-right+width+left-1u;cell=(c->map->origin)+top*228u+left;
    if((int32_t)top<=(int32_t)bottom)for(row=top;;++row){
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{
                flags=c->map->terrain[cell];
                if(!(flags&0x10004u)&&!(uint8_t)pharaoh_map_terrain_mask_has_open_corner(c->map->terrain,cell,0x10004u,1)){
                    flags=c->map->terrain[cell];c->map->terrain[cell]=(flags&0xd9cbf680u)|0x10000u;changed=1;
                    pharaoh_map_refresh_painted_region(c,(int32_t)col,(int32_t)row,2);pharaoh_map_extend_floodplain(c,(int32_t)col,(int32_t)row);
                }
                ++col;++cell;
            }while((int32_t)col<=(int32_t)right);
        }
        cell+=step;if((int32_t)(row+1u)>(int32_t)bottom)break;
    }
    return (uint8_t)changed;
}

uint8_t pharaoh_map_remove_blocked_floodplain(PharaohMapSurfaceContext *c,int32_t xIn,int32_t yIn)
{
    uint32_t x=(uint32_t)xIn,y=(uint32_t)yIn,left=x-1u,right,width,top=y-1u,bottom,step,cell,row,col,flags,changed=0;
    width=(c->map->width);right=width;if((int32_t)left<0)left=0;if((int32_t)right>=(int32_t)(x+1u))right=x+1u;
    bottom=(c->map->height);if((int32_t)top<0)top=0;if((int32_t)bottom>=(int32_t)(y+1u))bottom=y+1u;
    step=(*c->border_size)-right+width+left-1u;cell=(c->map->origin)+top*228u+left;
    if((int32_t)top>(int32_t)bottom)return 0;
    for(row=top;;++row){
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{
                flags=c->map->terrain[cell];
                if(flags&0x10000u){
                    c->map->terrain[cell]=flags&0xfffeffffu;changed=1;pharaoh_map_refresh_painted_region(c,(int32_t)col,(int32_t)row,1);
                    flags=c->map->terrain[cell];if(flags&0x22341a33u)pharaoh_map_remove_blocked_floodplain(c,(int32_t)col,(int32_t)row);
                }
                ++col;++cell;
            }while((int32_t)col<=(int32_t)right);
        }
        cell+=step;if((int32_t)(row+1u)>(int32_t)bottom)break;
    }
    return (uint8_t)changed;
}

uint8_t pharaoh_map_trim_floodplain_spurs(PharaohMapSurfaceContext *c,int32_t x,int32_t y)
{
    uint32_t offset=(c->map->origin),cell=offset+(uint32_t)y*228u+(uint32_t)x,next,flags;int32_t Direction,relative,nx,ny;
    if(c->map->terrain[cell]&0x10000u)return 0;
    Direction=pharaoh_map_terrain_least_connected_neighbor(c->map->terrain,cell,0x10000u);if(!Direction)return 0;
    do{
        next=cell+(uint32_t)Direction;offset=(c->map->origin);flags=c->map->terrain[next];c->map->terrain[next]=(flags&0xfffeffffu)|128u;
        relative=(int32_t)(next-offset);nx=relative%228;ny=relative/228;
        pharaoh_map_trim_floodplain_spurs(c,nx,ny);pharaoh_map_refresh_painted_region(c,nx,ny,1);Direction=pharaoh_map_terrain_least_connected_neighbor(c->map->terrain,cell,0x10000u);
    }while(Direction);
    return 1;
}

void pharaoh_map_normalize_floodplain_region(PharaohMapSurfaceContext *c,int32_t xmin,int32_t ymin,int32_t xmax,int32_t ymax)
{
    uint32_t left=(uint32_t)xmin,top=(uint32_t)ymin,right=(uint32_t)xmax,bottom=(uint32_t)ymax;
    uint32_t width,height,step,cell,row,column,flags;
    if(xmin<0)left=0;if(ymin<0)top=0;
    width=(c->map->width);if(((int32_t)(right))>=((int32_t)(width)))right=width-1u;
    height=(c->map->height);if(((int32_t)(bottom))>=((int32_t)(height)))bottom=height-1u;
    step=width-right+left-1u;cell=(c->map->origin)+top*228u+left;
    for(row=top;((int32_t)(row))<=((int32_t)(bottom));++row){
        for(column=left;((int32_t)(column))<=((int32_t)(right));++column,++cell){
            flags=c->map->terrain[cell];
            if(flags&4u)(void)pharaoh_native_return_zero_byte_00477f60();
            else if(flags&0x10000u)(void)pharaoh_map_extend_floodplain(c,((int32_t)(column)),((int32_t)(row)));
            else if(flags&0x22341a33u)(void)pharaoh_map_remove_blocked_floodplain(c,((int32_t)(column)),((int32_t)(row)));
            else (void)pharaoh_map_trim_floodplain_spurs(c,((int32_t)(column)),((int32_t)(row)));
        }
        cell+=(*c->border_size)+step;
    }
}

void pharaoh_map_normalize_floodplain_all(PharaohMapSurfaceContext *c)
{
    uint32_t bottom=(c->map->height)-1u,right=(c->map->width)-1u;
    pharaoh_map_normalize_floodplain_region(c,0,0,(int32_t)right,(int32_t)bottom);
}

void pharaoh_map_refresh_painted_region(PharaohMapSurfaceContext *c,int32_t xIn,int32_t yIn,int32_t radiusIn)
{
    uint32_t x=(uint32_t)xIn,y=(uint32_t)yIn,r=(uint32_t)radiusIn;
    uint32_t right=(c->map->width),left=x-r,top=y-r,bottom;
    if((int32_t)left<0)left=0;if((int32_t)right>=(int32_t)(x+r))right=x+r;
    bottom=(c->map->height);if((int32_t)top<0)top=0;if((int32_t)bottom>=(int32_t)(y+r))bottom=y+r;
    pharaoh_map_refresh_water(c,(int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom,1);
    pharaoh_map_refresh_marshland(c,(int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom);
    pharaoh_map_refresh_faults(c,(int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom);
    pharaoh_map_refresh_rocks(c);
    pharaoh_map_refresh_dune_region(c,(int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom);
    pharaoh_map_refresh_empty_region(c,(int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom,0,1);
    pharaoh_map_refresh_floodplain(c,(int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom);
    pharaoh_map_refresh_road_region(c,(int32_t)(left-1u),(int32_t)(top-1u),(int32_t)(right+1u),(int32_t)(bottom+1u));
    pharaoh_map_update_channel_images(c,(int32_t)(left-1u),(int32_t)(top-1u),(int32_t)(right+1u),(int32_t)(bottom+1u));
    pharaoh_map_refresh_meadow(c,(int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom);
}
