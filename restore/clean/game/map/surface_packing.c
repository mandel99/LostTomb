#include "surface_refresh_internal.h"

void pharaoh_map_refresh_gardens(PharaohMapSurfaceContext *c)
{
 uint32_t height=(c->map->height),row=0,left,limit,cell,remaining,flags,x,variation,image;
 uint8_t Access,sprite;
 /* The first pass caches height across every row and reads the access BYTE
    before clearing the image DWORD. The second pass has separate row caches. */
 for(row=0;((int32_t)(row))<((int32_t)(height));++row){
  left=c->row_x_start[row];limit=c->row_x_end[row];cell=c->row_start_offset[row];
  if(((int32_t)(left))>=((int32_t)(limit)))continue;
  remaining=limit-left;
  do{
   flags=c->map->terrain[cell];
   if((flags&0x20u)&&!(flags&0x600u)){
    Access=c->property_flags[cell];c->image_ids[cell]=0;c->property_flags[cell]=(uint8_t)(Access&0xf0u);
    sprite=c->edge_flags[cell];c->edge_flags[cell]=(uint8_t)(sprite|64u);
   }
   ++cell;--remaining;
  }while(remaining);
 }
 row=0;
 while(((int32_t)(row))<((int32_t)(height))){
  x=c->row_x_start[row];limit=c->row_x_end[row];cell=c->row_start_offset[row];
  if(((int32_t)(x))<((int32_t)(limit))){
   do{
    flags=c->map->terrain[cell];
    if((flags&0x20u)&&!(flags&0x600u)&&!c->image_ids[cell]){
     if(surface_rectangle_fits(c,((int32_t)(x)),((int32_t)(row)),3,3,0x20,1)){
      image=c->groups[0x148].first_image+7u;pharaoh_map_place_surface_square(c,0,((int32_t)(x)),((int32_t)(row)),3,3,image,0x20);
     }else if(surface_rectangle_fits(c,((int32_t)(x)),((int32_t)(row)),2,2,0x20,1)){
      variation=c->random[cell]&3u;image=c->groups[0x148].first_image+(variation<=1u?6u:variation==2u?5u:4u);
      pharaoh_map_place_surface_square(c,0,((int32_t)(x)),((int32_t)(row)),2,2,image,0x20);
     }else{
      image=c->groups[0x148].first_image+(x&1u)+(row&1u)*2u;c->image_ids[cell]=image;
     }
    }
    ++x;++cell;
   }while(((int32_t)(x))<((int32_t)(limit)));
   /* Empty rows do not reload the native cached height. */
   height=(c->map->height);
  }
  ++row;
 }
}

void pharaoh_map_refresh_rocks(PharaohMapSurfaceContext *c)
{
    uint32_t height=(c->map->height),row,left,right,cell,count,flags,Access,sprite,col,mask,base,variation;
    int32_t variant;
    if((int32_t)height<=0)return;
    for(row=0;(int32_t)row<(int32_t)height;++row){
        left=c->row_x_start[row];right=c->row_x_end[row];cell=c->row_start_offset[row];
        if((int32_t)left<(int32_t)right){
            count=right-left;
            do{
                flags=c->map->terrain[cell];
                if(flags&2u){
                    Access=c->property_flags[cell];
                    if(!(Access&128u)&&!(flags&0x600u)){
                        c->image_ids[cell]=0;c->property_flags[cell]=(uint8_t)(Access&0xf0u);
                        sprite=c->edge_flags[cell];c->edge_flags[cell]=(uint8_t)(sprite|64u);
                    }
                }
                ++cell;--count;
            }while(count);
        }
    }
    row=0;
    do{
        left=c->row_x_start[row];right=c->row_x_end[row];cell=c->row_start_offset[row];
        if((int32_t)left<(int32_t)right){
            col=left;
            do{
                flags=c->map->terrain[cell];
                if((flags&2u)&&!(c->property_flags[cell]&128u)&&!(flags&0x600u)&&!c->image_ids[cell]){
                    if((flags&0x100002u)==0x100002u){mask=0x100002u;base=c->groups[0x21b].first_image;}
                    else if((flags&0x20000002u)==0x20000002u){
                        base=c->groups[0x2e7].first_image;variant=surface_rock_variant(c,cell);
                        if(variant==-1){variation=c->random[cell]&7u;base=c->groups[0x1f9].first_image;c->image_ids[cell]=variation+base;}
                        else c->image_ids[cell]=(uint32_t)variant+base;
                        goto nextCell;
                    }else{mask=2u;base=c->groups[0x1f9].first_image;}
                    if(surface_rectangle_fits(c,(int32_t)col,(int32_t)row,3,3,mask,1)){
                        variation=c->random[cell]&1u;pharaoh_map_place_surface_square(c,0,(int32_t)col,(int32_t)row,3,3,base+variation+12u,mask);
                    }else if(surface_rectangle_fits(c,(int32_t)col,(int32_t)row,2,2,mask,1)){
                        variation=c->random[cell]&3u;pharaoh_map_place_surface_square(c,0,(int32_t)col,(int32_t)row,2,2,base+variation+8u,mask);
                    }else{variation=c->random[cell]&7u;c->image_ids[cell]=base+variation;}
                }
nextCell:      ++col;++cell;
            }while((int32_t)col<(int32_t)right);
            height=(c->map->height);
        }
        ++row;
    }while((int32_t)row<(int32_t)height);
}

void pharaoh_map_refresh_dunes(PharaohMapSurfaceContext *c)
{
    uint32_t height=(c->map->height),row=0,left,right,cell,count,flags,Access,sprite,col,size,variation,base;
    if((int32_t)height>0)do{
        left=c->row_x_start[row];right=c->row_x_end[row];cell=c->row_start_offset[row];
        if((int32_t)left<(int32_t)right){
            count=right-left;
            do{
                flags=c->map->terrain[cell];
                if(flags&0x2000000u){
                    Access=c->property_flags[cell];
                    if(!(Access&128u)&&!(flags&0x600u)){
                        c->image_ids[cell]=0;c->property_flags[cell]=(uint8_t)(Access&0xf0u);
                        sprite=c->edge_flags[cell];c->edge_flags[cell]=(uint8_t)(sprite|64u);
                    }
                }
                ++cell;--count;
            }while(count);
        }
        height=(c->map->height);++row;
    }while((int32_t)row<(int32_t)height);
    height=(c->map->height);row=0;
    if((int32_t)height<=0)return;
    do{
        left=c->row_x_start[row];right=c->row_x_end[row];cell=c->row_start_offset[row];
        if((int32_t)left<(int32_t)right){
            col=left;
            do{
                flags=c->map->terrain[cell];
                if((flags&0x2000000u)&&!(c->property_flags[cell]&128u)&&!(flags&0x600u)&&!c->image_ids[cell]){
                    if(surface_rectangle_fits(c,(int32_t)col,(int32_t)row,3,3,0x2000000u,1))size=3;
                    else if(surface_rectangle_fits(c,(int32_t)col,(int32_t)row,2,2,0x2000000u,1))size=2;
                    else size=1;
                    /* Both original query-result branches perform the same reads
                       and stores. Keep the genuine query and its state effects. */
                    surface_terrain_near(c,(int32_t)col,(int32_t)row,(int32_t)size,4,0x200u);
                    variation=c->random[cell];base=c->groups[0x1fe].first_image;
                    if(size==3)pharaoh_map_place_surface_square(c,0,(int32_t)col,(int32_t)row,3,3,base+(variation&1u)+12u,0x2000000u);
                    else if(size==2)pharaoh_map_place_surface_square(c,0,(int32_t)col,(int32_t)row,2,2,base+(variation&3u)+8u,0x2000000u);
                    else c->image_ids[cell]=base+(variation&7u);
                }
                ++col;++cell;
            }while((int32_t)col<(int32_t)right);
        }
        height=(c->map->height);++row;
    }while((int32_t)row<(int32_t)height);
}
