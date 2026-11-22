#include "surface_refresh_internal.h"

void pharaoh_map_refresh_road_region(PharaohMapSurfaceContext *c,int32_t xmin,int32_t ymin,int32_t xmax,int32_t ymax)
{
    int32_t width,height;uint32_t tile,step,column,row,flags,padding;
    if(xmin<0)xmin=0;if(ymin<0)ymin=0;
    width=(int32_t)(c->map->width);if(xmax>=width)xmax=(int32_t)((uint32_t)width-1u);
    height=(int32_t)(c->map->height);if(ymax>=height)ymax=(int32_t)((uint32_t)height-1u);
    ((*c->scan_y) = ((uint32_t)ymin));step=(uint32_t)width-(uint32_t)xmax+(uint32_t)xmin-1u;
    tile=(c->map->origin)+(uint32_t)ymin*228u+(uint32_t)xmin;((*c->current_tile) = (tile));if(ymin>ymax)return;
    do{
        ((*c->scan_x) = ((uint32_t)xmin));
        if(xmin<=xmax){do{
            flags=c->map->terrain[tile];
            if((flags&0x40u)&&!(flags&0xcu)){pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,(int32_t)*c->current_tile,0x8000040u);pharaoh_map_adjacent_add_gatehouse_flags(c);pharaoh_map_set_road_image(c);tile=(*c->current_tile);}
            column=(*c->scan_x)+1u;++tile;((*c->scan_x) = (column));((*c->current_tile) = (tile));
        }while((int32_t)column<=xmax);}
        padding=(*c->border_size);row=(*c->scan_y)+1u;tile+=padding+step;((*c->scan_y) = (row));((*c->current_tile) = (tile));
    }while((int32_t)row<=ymax);
}

void pharaoh_map_set_road_image(PharaohMapSurfaceContext *c)
{
    PharaohFloodTile *instance;
    uint32_t tile=(*c->current_tile),flags,mode,frame,base,direction,sprite,saved,old,attribute,a,b;
    if(c->building_ids[tile]!=0)return;
    if(c->property_flags[tile]&0x80u)return;
    mode=(*c->force_paved_roads);
    if(mode==1){
        surface_match(c,c->paved_road_context,48,-1,0);pharaoh_map_adjacent_flags_normalize(c->adjacent);frame=(c->selection->frame);base=c->groups[0x212].first_image;direction=(c->selection->orientation_offset);tile=(*c->current_tile);sprite=base+frame+direction;
    }else{
        flags=c->map->terrain[tile];
        if(flags&0x100u){
            old=c->image_ids[tile];base=c->groups[0x1fa].first_image;saved=(int32_t)(old-base)>=48?48u:0u;
            pharaoh_map_adjacent_channel_flags(c);surface_match(c,c->channel_context,16,-1,0);attribute=(c->selection->record_value);
            if(attribute==0){tile=(*c->current_tile);a=c->map->terrain[(tile-1u)];if(a&0x100u)((c->selection->orientation_offset) = (0));else{a=c->map->terrain[(tile+1u)];((c->selection->orientation_offset) = (1));if(a&0x100u)((c->selection->orientation_offset) = (0));}}
            if(pharaoh_map_last_tile_is_buildable_for_editor(c->map->terrain,c->height_values,(int32_t)*c->current_tile)){
                direction=(c->selection->orientation_offset);base=c->groups[0x1fa].first_image;tile=(*c->current_tile);sprite=base+direction+saved+42u;
            }else{
                direction=(c->selection->orientation_offset);base=c->groups[0x1fa].first_image;sprite=base+direction+saved+15u;tile=(*c->current_tile);
                if(c->map->terrain[tile]&0x10000u){sprite=(uint32_t)pharaoh_map_road_canal_floodplain_image((int32_t)sprite);tile=(*c->current_tile);}
            }
        }else if(!(flags&0x10000u)&&(!(instance=c->flood_tiles[tile])||instance->is_floodplain!=0)){
            if(pharaoh_map_last_tile_is_buildable_for_editor(c->map->terrain,c->height_values,(int32_t)*c->current_tile)){
                surface_match(c,c->paved_road_context,48,-1,0);pharaoh_map_adjacent_flags_normalize(c->adjacent);frame=(c->selection->frame);base=c->groups[0x212].first_image;direction=(c->selection->orientation_offset);sprite=base+frame+direction;tile=(*c->current_tile);
            }else{
                surface_match(c,c->dirt_road_context,17,-1,0);pharaoh_map_adjacent_flags_normalize(c->adjacent);frame=(c->selection->frame);base=c->groups[0x21c].first_image;direction=(c->selection->orientation_offset);tile=(*c->current_tile);sprite=base+frame+direction;
            }
        }else{
            old=c->image_ids[tile];base=c->groups[0x210].first_image;saved=old-base;sprite=(uint32_t)pharaoh_map_road_canal_crossing_image(tile,(int32_t)saved);
            if(sprite!=saved||((int32_t)saved>=84&&(int32_t)saved<=87)){base=c->groups[0x210].first_image;sprite+=base;}
            else{surface_match(c,c->dirt_road_context,17,-1,0);pharaoh_map_adjacent_flags_normalize(c->adjacent);frame=(c->selection->frame);base=c->groups[0x224].first_image;sprite=frame+base;direction=(c->selection->orientation_offset);sprite+=direction;}
            tile=(*c->current_tile);
        }
    }
    (c->image_ids[tile] = (sprite));a=c->property_flags[tile];b=c->edge_flags[tile];(c->property_flags[tile] = (uint8_t)(a&0xf0u));(c->edge_flags[tile] = (uint8_t)(b|0x40u));
}

void pharaoh_map_refresh_roads(PharaohMapSurfaceContext *c)
{pharaoh_map_refresh_road_region(c,0,0,c->map->width-1,c->map->height-1);}
