#include "surface_refresh_internal.h"
#include "pharaoh/map/moisture.h"
#include "pharaoh/core/static_objects.h"

static uint32_t bounded_growth(PharaohMapSurfaceContext *c,uint32_t cell,uint32_t value)
{
    PharaohFloodTile *object=c->flood_tiles[cell];
    if(object&&object->is_floodplain==0)return 100;
    if(c->map->terrain[cell]&0x40004u)return 100;
    return value<128u?value:0;
}

void pharaoh_map_rebuild_fertility(PharaohMapSurfaceContext *c,int32_t leftIn,int32_t topIn,int32_t rightIn,int32_t bottomIn)
{
    uint32_t left=(uint32_t)leftIn,top=(uint32_t)topIn,right=(uint32_t)rightIn,bottom=(uint32_t)bottomIn;
    uint32_t width,height,saved,step,padding,Climate,limit,offset,cell,row,col,flags,denominator,arg,orientation,variant;
    int32_t water,high,a,b,v,threshold;
    if(leftIn<0)left=0;if(topIn<0)top=0;
    width=(c->map->width);if((int32_t)right>=(int32_t)width)right=width-1u;
    height=(c->map->height);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    saved=(*c->current_tile);padding=(*c->border_size);step=padding-right+width+left-1u;
    Climate=((uint8_t)*c->climate);limit=Climate==0?46u:Climate==2?8u:16u;
    offset=(c->map->origin);cell=offset+top*228u+left;
    if((int32_t)top<=(int32_t)bottom)for(row=top;;++row){
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{
                flags=c->map->terrain[cell];
                if(flags&0x50004u){c->fertility[cell]=0;c->map->terrain[cell]=flags&0xffffff7fu;}
                else{
                    water=pharaoh_map_nearest_terrain_ring(c,(int32_t)col,(int32_t)row,4,(int32_t)limit);
                    high=pharaoh_map_nearest_terrain_ring(c,(int32_t)col,(int32_t)row,0x50000u,8);
                    if(water<0&&high<0){flags=c->map->terrain[cell];c->map->terrain[cell]=flags&0xffffff7fu;c->fertility[cell]=0;}
                    else{
                        a=0;
                        if(water>=0){
                            threshold=(int32_t)(limit-12u);v=(int32_t)((uint32_t)water-(uint32_t)threshold+1u);denominator=limit;
                            if(water<threshold)water=0;else if(v>=0){water=v;denominator=12;}
                            arg=(uint32_t)water-1u;if((int32_t)arg<=0)arg=0;
                            a=pharaoh_map_fertility_at_distance((int32_t)arg,(int32_t)denominator);
                        }
                        if(high<0)b=0;
                        else{arg=(uint32_t)high-1u;if((int32_t)arg<=0)arg=0;b=pharaoh_map_fertility_at_distance((int32_t)arg,8);}
                        if(a>b)b=a;if(b<1)b=1;
                        flags=c->map->terrain[cell];c->map->terrain[cell]=flags|128u;c->fertility[cell]=(uint8_t)b;
                    }
                }
                ++col;++cell;
            }while((int32_t)col<=(int32_t)right);
        }
        cell+=step;if((int32_t)(row+1u)>(int32_t)bottom)break;
    }
    offset=(c->map->origin);cell=offset+top*228u+left;
    if((int32_t)top<=(int32_t)bottom)for(row=top;;++row){
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{
                if(((uint8_t)c->map->terrain[cell]&128u)&&pharaoh_map_adjust_moisture_neighbors(c,cell)){
                    flags=c->map->terrain[cell];orientation=(*c->orientation);
                    c->map->terrain[cell]=flags&0xfffff7ffu;(*c->orientation)=0;
                    surface_match(c,c->floodplain_edges,26,-1,0);variant=(c->selection->orientation_offset);
                    c->fertility[cell]=variant==14u?100u:(uint8_t)(variant|128u);(*c->orientation)=orientation;
                }
                ++col;++cell;
            }while((int32_t)col<=(int32_t)right);
        }
        cell+=step;if((int32_t)(row+1u)>(int32_t)bottom)break;
    }
    (*c->current_tile)=saved;
}

uint32_t pharaoh_map_adjust_moisture_neighbors(PharaohMapSurfaceContext *c,uint32_t cell)
{
    static const uint32_t delta[8]={0xffffff1c,0xffffff1d,1,229,228,227,0xffffffff,0xffffff1b};
    uint32_t n=cell-228u,flags,level,i,value;
    flags=(uint8_t)c->map->terrain[n];
    if((flags&4u)&&((uint8_t)c->map->terrain[(cell)+228u]&4u)){c->fertility[cell]=100;return 0;}
    if(((uint8_t)c->map->terrain[(cell)-1u]&4u)&&((uint8_t)c->map->terrain[(cell)+1u]&4u)){c->fertility[cell]=100;return 0;}
    if(pharaoh_map_moisture_flood_objects_around(c->flood_tiles,(int32_t)cell)){c->fertility[cell]=100;return 0;}
    level=c->fertility[cell];
    if(level>90u||level<=40u){if(level==100u)c->fertility[cell]=100;return 0;}
    for(i=0;i<4u;++i){
        n=cell+delta[i];
        if((uint8_t)pharaoh_map_opposite_pair_has_mask(c->map->terrain,(int32_t)n,4u)||pharaoh_map_moisture_flood_objects_around(c->flood_tiles,(int32_t)n))value=100;
        else value=c->fertility[n];
        value=bounded_growth(c,n,value);c->adjacent[i]=value==100u;
    }
    for(i=4;i<7u;++i){
        n=cell+delta[i];value=pharaoh_map_moisture_or_wet(c->map->terrain,c->flood_tiles,c->fertility,(int32_t)n);value=bounded_growth(c,n,value);c->adjacent[i]=value==100u;
    }
    value=pharaoh_map_moisture_neighbour_shade(c->map->terrain,c->flood_tiles,c->fertility,(int32_t)cell,-229);(c->adjacent[7])=value==100u;
    return (uint32_t)pharaoh_map_adjacent_flags_count(c->adjacent);
}

void pharaoh_map_revalidate_water_images(PharaohMapSurfaceContext *c)
{
    uint32_t right,bottom;
    if(!(*c->water_revalidation_pending))return;
    pharaoh_map_normalize_floodplain_all(c);
    bottom=(c->map->height)-1u;right=(c->map->width)-1u;
    pharaoh_map_rebuild_fertility(c,0,0,(int32_t)right,(int32_t)bottom);
    bottom=(c->map->height)-1u;right=(c->map->width)-1u;
    pharaoh_map_update_deepwater(c,0,0,(int32_t)right,(int32_t)bottom);
    (*c->water_revalidation_pending)=0;
}
