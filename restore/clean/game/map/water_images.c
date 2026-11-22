#include "surface_refresh_internal.h"

void pharaoh_map_set_water_image(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t set)
{
    uint32_t oy=(c->geometry->diamond_y_min),ox=(c->geometry->diamond_x_min),tile,flags,saved,water,base,frame,sprite,variant,rot=0,level,other,mark;PharaohFloodTile *ptr;
    int32_t left,top,cols=3,rows=3,endy,endx,row,col,count,Code;
    uint8_t inactive[8];uint32_t i;
    static const int32_t around[8]={-228,-227,1,229,228,227,-1,-229};
    if(!(uint8_t)pharaoh_map_grid_is_inside(c->geometry,(int32_t)((uint32_t)x+ox),(int32_t)((uint32_t)y+oy)))return;
    tile=(c->map->origin)+(uint32_t)y*228u+(uint32_t)x;
    flags=c->map->terrain[tile];flags=set==1?flags|4u:flags&0xfbfffffbu;
    left=(int32_t)((uint32_t)x-1u);top=(int32_t)((uint32_t)y-1u);
    (c->map->terrain[tile] = (flags));
    if(left<0){cols=(int32_t)((uint32_t)left+3u);left=0;}else{int32_t width=(int32_t)(c->map->width);if((int32_t)((uint32_t)left+3u)>width)cols=(int32_t)((uint32_t)width-(uint32_t)left);}
    if(top<0){rows=(int32_t)((uint32_t)top+3u);top=0;}else{int32_t height=(int32_t)(c->map->height);if((int32_t)((uint32_t)top+3u)>height)rows=(int32_t)((uint32_t)height-(uint32_t)top);}
    water=(uint8_t)pharaoh_map_sweep_has_passed(*c->sweep_countdown,*c->sweep_start,left);saved=(*c->current_tile);
    endy=(int32_t)((uint32_t)top+(uint32_t)rows);tile=(c->map->origin)+(uint32_t)top*228u+(uint32_t)left;
    for(row=top;row<endy;row=(int32_t)((uint32_t)row+1u)){
        endx=(int32_t)((uint32_t)left+(uint32_t)cols);
        for(col=left;col<endx;col=(int32_t)((uint32_t)col+1u),++tile){
            flags=c->map->terrain[tile];if(!(flags&4u)||(flags&8u))continue;
            ((*c->current_tile) = (tile));pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,(int32_t)*c->current_tile,4);pharaoh_map_adjacent_flags_invert(c->adjacent);
            surface_match(c,c->water_context,48,(int32_t)c->random[tile],0);
            flags=c->map->terrain[tile];
            if(flags&0x10000u){
                variant=(c->selection->orientation_offset);if((int32_t)variant<8)goto fallback;
                if(water){base=c->groups[0x2cf].first_image;frame=(c->selection->frame);}else{frame=(c->selection->frame);base=c->groups[0x204].first_image;}
                sprite=base+frame+variant;goto signcheck;
            }
            ((*c->current_tile) = (tile));pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,(int32_t)*c->current_tile,0x10000);count=pharaoh_map_adjacent_flags_count(c->adjacent);
            if(!count){variant=(c->selection->orientation_offset);goto fallback;}
            other=(*c->current_tile);
            for(i=0;i<8;++i){ptr=c->flood_tiles[(other+(uint32_t)around[i])];inactive[i]=(uint8_t)(ptr!=0&&ptr->is_floodplain==0);}
            level=(uint32_t)(count<2?count:2)*2u;variant=(c->selection->orientation_offset);Code=-1;
            switch(variant){
            case 8:case 12:case 16:case 20:
                rot=(*c->orientation);i=variant==8?rot-4u:variant==12?rot-2u:variant==16?rot:rot+2u;
                if(inactive[i&7u])Code=variant==8?4:variant==12?6:variant==16?0:2;
                level=0;if(Code<0)goto defaultRoad;break;
            case 24:case 28:case 32:case 36:
                rot=(*c->orientation);
                i=(variant==24||variant==28)?rot-2u:rot+2u;
                if(inactive[i&7u])Code=(variant==24||variant==28)?6:2;
                else {i=(variant==24||variant==36)?rot-4u:rot;if(!inactive[i&7u])goto defaultRoad;Code=(variant==24||variant==36)?4:0;}
                if(level==2){other=(c->adjacent[6]);other|=(c->adjacent[4]);other|=(c->adjacent[2]);other|=(c->adjacent[0]);if(other)level=4;}break;
            default:goto defaultRoad;
            }
            base=c->groups[0x210].first_image;sprite=base+3u*(((uint32_t)Code-4u)&7u)+60u+level;
            if(c->adjacent[(rot+(uint32_t)Code+1u)&7u])++sprite;else if(c->adjacent[(rot+(uint32_t)Code+2u)&7u])++sprite;
            if(water){other=sprite-base;if((int32_t)other>=60&&(int32_t)other<=83)sprite+=c->groups[0x2d0].first_image-base-60u;}
            if((int32_t)sprite>=0)goto publish;
defaultRoad:
            frame=(c->selection->frame);base=(water?c->groups[0x2cf].first_image:c->groups[0x204].first_image);sprite=base+frame+variant;
            ((*c->current_tile) = (tile));pharaoh_map_adjacent_terrain_include_exclude(c->adjacent,c->map->terrain,(int32_t)*c->current_tile,0x100,4);
            ((c->adjacent[7]) = (0));((c->adjacent[5]) = (0));((c->adjacent[3]) = (0));((c->adjacent[1]) = (0));
            count=pharaoh_map_adjacent_flags_count(c->adjacent);variant=(c->selection->orientation_offset);if(!count)goto signcheck;Code=-1;
            switch(variant){
            case 8:case 12:case 16:case 20:
                rot=(*c->orientation);i=variant==8?rot-4u:variant==12?rot-2u:variant==16?rot:rot+2u;
                if(!c->adjacent[(i)&7u])goto signcheck;Code=variant==8?3:variant==12?0:variant==16?1:2;break;
            default:
                rot=(*c->orientation);other=c->adjacent[(rot-2u)&7u];
                if(other&&c->adjacent[(rot-4u)&7u])Code=7;
                else if(other&&c->adjacent[(rot)&7u])Code=4;
                else if(c->adjacent[(rot+2u)&7u]){if(c->adjacent[(rot)&7u])Code=5;else if(c->adjacent[(rot-4u)&7u])Code=6;}
                if(variant==24){if(c->adjacent[(rot-4u)&7u])Code=11;else if(other)Code=8;}
                else if(variant==28){if(other)Code=13;else if(c->adjacent[(rot)&7u])Code=10;}
                else if(variant==32){if(c->adjacent[(rot+2u)&7u])Code=12;else if(c->adjacent[(rot)&7u])Code=15;}
                else if(variant==36){if(c->adjacent[(rot+2u)&7u])Code=9;else if(c->adjacent[(rot-4u)&7u])Code=14;}
                if(Code<0)goto signcheck;break;
            }
            sprite=(water?c->groups[0x2d6].first_image:c->groups[0x21d].first_image)+(uint32_t)Code;
signcheck:
            if((int32_t)sprite>=0)goto publish;
fallback:
            if(c->map->terrain[tile]&0x4000000u){
                pharaoh_map_image_context_reset(c->floodplain_edges,26);((*c->current_tile) = (tile));pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,(int32_t)*c->current_tile,0x4000000);pharaoh_map_adjacent_flags_invert(c->adjacent);
                other=(*c->current_tile);surface_match(c,c->floodplain_edges,26,(int32_t)c->random[other],1);
                if(water){frame=(c->selection->frame);base=c->groups[0x2ce].first_image;variant=(c->selection->orientation_offset);sprite=base+frame*15u+variant;}
                else if((uint8_t)pharaoh_map_sweep_matches_tile(*c->sweep_countdown,*c->sweep_start,left)){other=(*c->current_tile);frame=c->random[other]&3u;rot=(*c->orientation);base=c->groups[0x2ee].first_image;sprite=base+frame+(uint32_t)((int32_t)rot/2)*4u;}
                else{frame=(c->selection->frame);base=c->groups[0x22e].first_image;variant=(c->selection->orientation_offset);sprite=base+frame*15u+variant;}
            }else{
                base=(variant?(water?c->groups[0x2cd].first_image:c->groups[0x1f6].first_image):(water?c->groups[0x2cf].first_image:c->groups[0x204].first_image));frame=(c->selection->frame);sprite=base+frame+variant;
            }
publish:
            other=c->property_flags[tile];mark=c->edge_flags[tile];
            (c->edge_flags[tile] = (uint8_t)(mark|0x40u));(c->image_ids[tile] = (sprite));(c->property_flags[tile] = (uint8_t)(other&0xf0u));
        }
        tile+=228u-(uint32_t)cols;
    }
    ((*c->current_tile) = (saved));
}

void pharaoh_map_refresh_water(PharaohMapSurfaceContext *c,int32_t xmin,int32_t ymin,int32_t xmax,int32_t ymax,uint32_t flags)
{
 uint32_t width,height,rowstep,tile,scan,terrain;int32_t row,x;
 if((uint8_t)flags)pharaoh_map_update_deepwater(c,(int32_t)((uint32_t)xmin-5u),(int32_t)((uint32_t)ymin-5u),(int32_t)((uint32_t)xmax+5u),(int32_t)((uint32_t)ymax+5u));
 if(xmin<0)xmin=0;if(ymin<0)ymin=0;
 width=(c->map->width);if(xmax>=(int32_t)width)xmax=(int32_t)(width-1u);
 height=(c->map->height);if(ymax>=(int32_t)height)ymax=(int32_t)(height-1u);
 rowstep=width-(uint32_t)xmax+(uint32_t)xmin-1u;
 tile=(c->map->origin)+(uint32_t)ymin*228u+(uint32_t)xmin;
 row=ymin;
 if(row<=ymax){do{
  if(xmin<=xmax){
   scan=tile;tile+=(uint32_t)xmax-(uint32_t)xmin+1u;x=xmin;
   do{
    terrain=c->map->terrain[scan];
    if((terrain&4u)&&!(terrain&8u))pharaoh_map_set_water_image(c,x,row,1);
    x=(int32_t)((uint32_t)x+1u);++scan;
   }while(x<=xmax);
  }
  tile+=(*c->border_size)+rowstep;row=(int32_t)((uint32_t)row+1u);
 }while(row<=ymax);}
}
