#include "surface_refresh_internal.h"
#include "pharaoh/map/moisture.h"
#include "pharaoh/core/static_objects.h"

void pharaoh_map_set_dune_image(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t mode)
{
    uint32_t dy=(c->geometry->diamond_y_min),dx=(c->geometry->diamond_x_min),cell,flags,v,base,sprite,Access;
    if(!(uint8_t)pharaoh_map_grid_is_inside(c->geometry,(int32_t)(dx+(uint32_t)x),(int32_t)(dy+(uint32_t)y)))return;
    cell=(c->map->origin)+(uint32_t)y*228u+(uint32_t)x;(*c->current_tile)=cell;flags=c->map->terrain[cell];
    /* Native non-1 mode retains only bit02000000; intention unresolved. */
    if(mode!=1){c->map->terrain[cell]=flags&0x2000000u;return;}
    v=c->random[cell];base=c->groups[0x1fe].first_image;c->map->terrain[cell]=flags|0x2000000u;
    sprite=c->edge_flags[cell];c->image_ids[cell]=base+(v&7u);
    Access=c->property_flags[cell];c->property_flags[cell]=(uint8_t)(Access&0xf0u);c->edge_flags[cell]=(uint8_t)(sprite|64u);
}

void pharaoh_map_refresh_dune_region(PharaohMapSurfaceContext *c,int32_t leftIn,int32_t topIn,int32_t rightIn,int32_t bottomIn)
{
    uint32_t left=(uint32_t)leftIn,top=(uint32_t)topIn,right=(uint32_t)rightIn,bottom=(uint32_t)bottomIn,width,height,cell,stride,row,col,next,flags;
    if(leftIn<0)left=0;if(topIn<0)top=0;
    width=(c->map->width);if((int32_t)right>=(int32_t)width)right=width-1u;
    height=(c->map->height);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    cell=(c->map->origin);stride=width-right+left-1u;cell+=top*228u+left;
    row=top;
    if((int32_t)row>(int32_t)bottom)return;
    do{
        if((int32_t)left<=(int32_t)right){
            next=cell+right-left+1u;col=left;
            do{
                flags=c->map->terrain[cell];
                if((flags&0x2000000u)&&!(flags&0x600u))pharaoh_map_set_dune_image(c,(int32_t)col,(int32_t)row,1);
                ++cell;++col;
            }while((int32_t)col<=(int32_t)right);
            cell=next;
        }
        cell+=(*c->border_size)+stride;++row;
    }while((int32_t)row<=(int32_t)bottom);
}
