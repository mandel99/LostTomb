#include "surface_refresh_internal.h"

/* Complete typed reuse of MapImageContextAddCanalSide,0047EB60.
 * Native callers supply one of the cardinal slots0,2,4,6. */
void pharaoh_map_adjacent_add_channel_side(PharaohMapSurfaceContext *c,
    int32_t direction,uint32_t *count)
{
    static const int32_t offsets[4][5]={
        {-228,-1,1,-229,-227},{1,-228,228,-227,229},
        {228,-1,1,227,229},{-1,-228,228,-229,227}};
    const int32_t *side=offsets[direction/2];const uint32_t tile=*c->current_tile;
    const uint32_t current=c->map->terrain[tile],neighbor=c->map->terrain[tile+(uint32_t)side[0]];
    uint32_t connected;
    if(!(neighbor&0x100u))return;
    if(current&0x40u){
        if(neighbor&0x8000040u)return;
        connected=(c->map->terrain[tile+(uint32_t)side[1]]&0x8000040u) ||
            (c->map->terrain[tile+(uint32_t)side[2]]&0x8000040u);
    }else connected=!(neighbor&0x8000040u) ||
        (c->map->terrain[tile+(uint32_t)side[3]]&0x8000040u) ||
        (c->map->terrain[tile+(uint32_t)side[4]]&0x8000040u);
    if(connected){c->adjacent[direction]=1;++*count;}
}

/* Native0047E870. Initial floodplain water edges survive disconnected canals. */
void pharaoh_map_adjacent_channel_flags(PharaohMapSurfaceContext *c)
{
    static const int32_t delta[4]={-228,1,228,-1},opposite[4]={228,-1,-228,1};
    const uint32_t tile=*c->current_tile,current=c->map->terrain[tile];uint32_t count=0;
    for(int32_t i=7;i>=0;--i)c->adjacent[i]=0;
    if(current&0x10000u)for(int32_t i=0;i<4;++i)
        if(c->map->terrain[tile+(uint32_t)delta[i]]&4u)c->adjacent[i*2]=1;
    for(int32_t i=0;i<4;++i)pharaoh_map_adjacent_add_channel_side(c,i*2,&count);
    for(int32_t i=0;i<4;++i){
        const uint32_t neighbor=tile+(uint32_t)delta[i];
        if(!(c->map->terrain[neighbor]&8u))continue;
        const PharaohBuildingTypeRecord *b=&c->buildings[c->building_ids[neighbor]];
        if(b->building_type!=90 ||
            (c->map->terrain[tile+(uint32_t)opposite[i]]&current&0x40u))continue;
        const uint8_t axis=b->reserved_aa[0]&1u;
        if((i&1)?axis!=0:axis==0)c->adjacent[i*2]=1;
    }
}

/* Native0047BE30 has a different north-road mask from0047BAE0. */
void pharaoh_map_set_channel_image(PharaohMapSurfaceContext *c,int32_t full)
{
    uint32_t tile=*c->current_tile;
    if(c->building_ids[tile])return;
    uint32_t water=0;
    const volatile uint32_t *image=&c->image_ids[tile];
    const uint32_t base=c->groups[0x1fa].first_image;
    /* Possible original bug: both signed comparisons cannot be true for
     * an unchanged image. Preserve the two native reads, do not change AND. */
    if((int32_t)(*image-base)<0 && (int32_t)(*image-base)>=48)water=48;
    if(full)water=48;
    pharaoh_map_adjacent_channel_flags(c);
    (void)surface_match(c,c->channel_context,16,-1,0);
    tile=*c->current_tile;
    if(c->map->terrain[tile]&0x40u){
        c->property_flags[tile]&=0x7fu;
        if(!c->selection->record_value)
            c->selection->orientation_offset=(c->map->terrain[tile-228u]&0x8000040u)==0;
        const uint8_t paved=pharaoh_map_last_tile_is_buildable_for_editor(c->map->terrain,
            c->height_values,(int32_t)*c->current_tile);
        c->selection->orientation_offset+=paved?42u:15u;
    }
    tile=*c->current_tile;
    uint32_t result=c->groups[0x1fa].first_image+c->selection->frame+
        c->selection->orientation_offset+water;
    if(c->map->terrain[tile]&0x10000u)
        result=(uint32_t)pharaoh_map_road_canal_floodplain_image((int32_t)result);
    tile=*c->current_tile;
    c->image_ids[tile]=result;c->property_flags[tile]&=0xf0u;
    c->canal_water[tile]=(uint8_t)c->selection->record_value;c->edge_flags[tile]|=0x40u;
}

void pharaoh_map_update_channel_images(PharaohMapSurfaceContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
    if(x0<0)x0=0;if(y0<0)y0=0;
    if(x1>=c->map->width)x1=c->map->width-1;if(y1>=c->map->height)y1=c->map->height-1;
    const uint32_t tail=(uint32_t)c->map->width-(uint32_t)x1+(uint32_t)x0-1u;
    uint32_t tile=(uint32_t)c->map->origin+(uint32_t)y0*228u+(uint32_t)x0;
    *c->scan_y=y0;*c->current_tile=tile;
    if(y0>y1)return;
    do{
        *c->scan_x=x0;
        if(x0<=x1)do{
            const uint32_t flags=c->map->terrain[tile];
            if((flags&0x100u) && !(flags&12u) && !(c->property_flags[tile]&0x80u)){
                pharaoh_map_adjacent_channel_flags(c);pharaoh_map_set_channel_image(c,0);
                tile=*c->current_tile;
            }
            ++*c->scan_x;++tile;*c->current_tile=tile;
        }while(*c->scan_x<=x1);
        ++*c->scan_y;tile+=(uint32_t)*c->border_size+tail;*c->current_tile=tile;
    }while(*c->scan_y<=y1);
}

void pharaoh_map_refresh_canals(PharaohMapSurfaceContext *c)
{
 uint32_t height=(c->map->height),cell=(c->map->origin),width,x,y,padding,flags,offset,image;
 uint8_t Access,sprite;
 (*c->current_tile)=cell;(*c->scan_y)=0;
 if(((int32_t)(height))<=0)return;
 width=(c->map->width);
 do{
  (*c->scan_x)=0;
  if(((int32_t)(width))>0)do{
   flags=c->map->terrain[cell];
   if((flags&0x4000u)&&!(flags&8u)){
    pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,(int32_t)*c->current_tile,0x4000);pharaoh_map_adjacent_flags_invert(c->adjacent);(void)surface_match(c,c->canal_context,48,-1,0);
    cell=(*c->current_tile);Access=c->property_flags[cell];sprite=c->edge_flags[cell];
    c->property_flags[cell]=(uint8_t)(Access&0xf0u);offset=(c->selection->frame);c->edge_flags[cell]=(uint8_t)(sprite|64u);
    image=c->groups[0x197].first_image+offset;image+=(c->selection->orientation_offset);c->image_ids[cell]=image;
    if(pharaoh_map_adjacent_floodplain_cardinals(c->adjacent,c->map->terrain,(int32_t)*c->current_tile)){
     pharaoh_map_adjacent_add_canal_flags(c->adjacent,c->map->terrain,(int32_t)*c->current_tile);
     if(surface_match(c,c->flood_canal_context,15,-1,0)){
      offset=(c->selection->frame);image=c->groups[0x197].first_image;cell=(*c->current_tile);image+=offset;image+=(c->selection->orientation_offset);c->image_ids[cell]=image;
     }else pharaoh_actor_walking_no_operation();
    }
    pharaoh_actor_walking_no_operation();width=(c->map->width);
   }
   x=(*c->scan_x);cell=(*c->current_tile);++x;++cell;(*c->scan_x)=x;(*c->current_tile)=cell;
  }while(((int32_t)(x))<((int32_t)(width)));
  padding=(*c->border_size);y=(*c->scan_y);cell+=padding;height=(c->map->height);++y;
  (*c->current_tile)=cell;(*c->scan_y)=y;
 }while(((int32_t)(y))<((int32_t)(height)));
}

void pharaoh_map_refresh_channel_region(PharaohMapSurfaceContext *c,int32_t leftIn,int32_t topIn,int32_t rightIn,int32_t bottomIn,uint32_t refresh)
{
 uint32_t left=(uint32_t)leftIn,top=(uint32_t)topIn,right=(uint32_t)rightIn,bottom=(uint32_t)bottomIn;
 uint32_t swap,width,height,columns,cell,x,y,flags,difference,alternate,pattern,offset,image,result;
 uint8_t Access,sprite,animation;
 if(((int32_t)(left))>((int32_t)(right))){swap=left;left=right;right=swap;}if(((int32_t)(top))>((int32_t)(bottom))){swap=top;top=bottom;bottom=swap;}
 --left;++right;--top;++bottom;if(((int32_t)(left))<0)left=0;
 width=(c->map->width);if(((int32_t)(right))>=((int32_t)(width)))right=width-1u;if(((int32_t)(top))<0)top=0;
 height=(c->map->height);if(((int32_t)(bottom))>=((int32_t)(height)))bottom=height-1u;
 columns=right-left+1u;cell=(c->map->origin)+top*228u+left;(*c->scan_y)=top;(*c->current_tile)=cell;
 if(((int32_t)(top))>((int32_t)(bottom))){(*c->channels_refresh_pending)=0;return;}
 do{
  (*c->scan_x)=left;
  if(((int32_t)(left))<=((int32_t)(right)))do{
   flags=c->map->terrain[cell];
   if((flags&0x100u)&&!(flags&12u)&&c->canal_water[cell]<=15u){
    difference=c->image_ids[cell];difference-=c->groups[0x1fa].first_image;alternate=(((int32_t)(difference))<0||((int32_t)(difference))>=48)?48u:0u;if(refresh)alternate=48;
    pharaoh_map_adjacent_channel_flags(c);(void)surface_match(c,c->channel_context,16,-1,0);cell=(*c->current_tile);
    if((uint8_t)c->map->terrain[cell]&64u){
     Access=c->property_flags[cell];pattern=(c->selection->record_value);c->property_flags[cell]=(uint8_t)(Access&127u);
     if(!pattern){flags=c->map->terrain[(cell-228u)];(c->selection->orientation_offset)=(flags&64u)==0;}
     result=pharaoh_map_last_tile_is_buildable_for_editor(c->map->terrain,c->height_values,(int32_t)*c->current_tile);offset=(c->selection->orientation_offset)+(result?42u:15u);cell=(*c->current_tile);(c->selection->orientation_offset)=offset;
    }
    offset=(c->selection->frame);image=c->groups[0x1fa].first_image+offset;image+=(c->selection->orientation_offset);flags=c->map->terrain[cell];image+=alternate;
    if(flags&0x10000u){image=(uint32_t)pharaoh_map_road_canal_floodplain_image(((int32_t)(image)));cell=(*c->current_tile);}
    Access=c->property_flags[cell];c->image_ids[cell]=image;sprite=c->edge_flags[cell];c->property_flags[cell]=(uint8_t)(Access&0xf0u);
    animation=(uint8_t)(c->selection->record_value);c->edge_flags[cell]=(uint8_t)(sprite|64u);c->canal_water[cell]=animation;
   }
   x=(*c->scan_x)+1u;++cell;(*c->scan_x)=x;(*c->current_tile)=cell;
  }while(((int32_t)(x))<=((int32_t)(right)));
  y=(*c->scan_y)+1u;cell+=228u-columns;(*c->scan_y)=y;(*c->current_tile)=cell;
 }while(((int32_t)(y))<=((int32_t)(bottom)));
 (*c->channels_refresh_pending)=0;
}
