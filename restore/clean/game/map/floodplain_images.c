#include "surface_refresh_internal.h"
#include "pharaoh/core/x87_numeric.h"
static const double bank_first_scale=0.01,bank_second_scale=8.0;

void pharaoh_map_set_floodplain_image(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t set)
{
    uint32_t gy=(uint32_t)(c->geometry->diamond_y_min),gx=(uint32_t)(c->geometry->diamond_x_min),test,index,flags,cols=3,rows=3,base;
    int32_t left,top,limit,endx,endy;uint8_t water;
    PharaohFloodTile *instance;const PharaohFloodList *definition;uint32_t anchor,sprite,height,parity,variant,byteMark,byteSecondary,currentColumn,currentRow;
    int32_t worldx=(int32_t)(gx+(uint32_t)x),worldy=(int32_t)(gy+(uint32_t)y);
    test=pharaoh_map_grid_is_inside(c->geometry,worldx,worldy);if((uint8_t)test==0)return;
    index=(uint32_t)(c->map->origin)+(uint32_t)y*228u+(uint32_t)x;((*c->current_tile) = (index));
    flags=(uint32_t)c->map->terrain[index];if(set==1)flags|=0x10000u;else flags&=~0x10000u;
    left=(int32_t)((uint32_t)x-1u);top=(int32_t)((uint32_t)y-1u);(c->map->terrain[index] = (flags));
    if(left<0){cols=(uint32_t)left+3u;left=0;}else{limit=(int32_t)(c->map->width);if((int32_t)((uint32_t)left+3u)>limit)cols=(uint32_t)limit-(uint32_t)left;}
    if(top<0){rows=(uint32_t)top+3u;top=0;}else{limit=(int32_t)(c->map->height);if((int32_t)((uint32_t)top+3u)>limit)rows=(uint32_t)limit-(uint32_t)top;}
    water=pharaoh_map_sweep_has_passed(*c->sweep_countdown,*c->sweep_start,left);base=(uint32_t)(c->map->origin);endy=(int32_t)((uint32_t)top+rows);
    ((*c->scan_y) = ((uint32_t)top));index=base+(uint32_t)top*228u+(uint32_t)left;((*c->current_tile) = (index));if(top>=endy)return;
    endx=(int32_t)((uint32_t)left+cols);
    do{
        ((*c->scan_x) = ((uint32_t)left));
        if(left<endx){
            do{
                flags=(uint32_t)c->map->terrain[index];
                if((flags&0x10000u)&&!(flags&0x148u)){
                    pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,(int32_t)*c->current_tile,0x10000u);pharaoh_map_adjacent_flags_invert(c->adjacent);
                    index=(uint32_t)(*c->current_tile);instance=c->flood_tiles[index];
                    if(instance&&(uint8_t)instance->is_floodplain==0){surface_match(c,c->bank_context,24,0,1);}
                    else{
                        pharaoh_map_adjacent_terrain_add_bits(c->adjacent,c->map->terrain,(int32_t)*c->current_tile,4,1);
                        ((c->adjacent[7]) = (0));((c->adjacent[5]) = (0));((c->adjacent[3]) = (0));((c->adjacent[1]) = (0));
                        /* Complete484D70 consumes only the first two of the
                           native caller's three pushed arguments(base,24,2). */
                        surface_grouped_match(c,c->bank_context,24);
                    }
                    sprite=(uint32_t)c->groups[0x210].first_image;variant=(uint32_t)(c->selection->frame);((c->selection->orientation_offset) = (0));index=(uint32_t)(*c->current_tile);sprite+=variant;
                    instance=c->flood_tiles[index];
                    if(instance){
                        anchor=(uint32_t)instance->grid_offset;height=(uint32_t)c->meadow_value[anchor];variant=(uint32_t)pharaoh_x87_i32_multiply_pair((int32_t)height,&bank_first_scale,&bank_second_scale);
                        parity=(uint32_t)c->random[anchor]&1u;variant=(variant&~1u)|parity;
                        definition=instance->list;sprite+=(uint32_t)definition->native_04+variant*6u;
                    }
                    if(water){base=(uint32_t)c->groups[0x210].first_image;variant=sprite-base;if((int32_t)variant>=60&&(int32_t)variant<=83)sprite+=(uint32_t)c->groups[0x2d0].first_image-base-60u;}
                    byteSecondary=(uint32_t)c->property_flags[index];byteMark=(uint32_t)c->edge_flags[index];
                    (c->property_flags[index] = (uint8_t)(byteSecondary&0xf0u));(c->image_ids[index] = (sprite));(c->edge_flags[index] = (uint8_t)(byteMark|0x40u));(c->canal_water[index] = (uint8_t)(0));
                }
                currentColumn=(uint32_t)(*c->scan_x)+1u;++index;((*c->scan_x) = (currentColumn));((*c->current_tile) = (index));
            }while((int32_t)currentColumn<endx);
        }
        currentRow=(uint32_t)(*c->scan_y)+1u;((*c->scan_y) = (currentRow));index+=228u-cols;((*c->current_tile) = (index));
    }while((int32_t)currentRow<endy);
}

void pharaoh_map_refresh_floodplain(PharaohMapSurfaceContext *c,int32_t xmin,int32_t ymin,int32_t xmax,int32_t ymax)
{
    int32_t width,height,x,y;uint32_t index,next,tile,step,flags;
    if(xmin<0)xmin=0;if(ymin<0)ymin=0;
    width=(int32_t)(c->map->width);if(xmax>=width)xmax=(int32_t)((uint32_t)width-1u);
    height=(int32_t)(c->map->height);if(ymax>=height)ymax=(int32_t)((uint32_t)height-1u);
    step=(uint32_t)width-(uint32_t)xmax+(uint32_t)xmin-1u;
    index=(uint32_t)(c->map->origin)+(uint32_t)ymin*228u+(uint32_t)xmin;
    for(y=ymin;y<=ymax;++y){
        if(xmin<=xmax){
            tile=index;next=index+(uint32_t)xmax-(uint32_t)xmin+1u;
            for(x=xmin;x<=xmax;++x,++tile){flags=(uint32_t)c->map->terrain[tile];if((flags&0x10000u)&&!(flags&8u))pharaoh_map_set_floodplain_image(c,x,y,1);}
            index=next;
        }
        index+=(uint32_t)(*c->border_size)+step;
    }
}

void pharaoh_map_set_marshland_image(PharaohMapSurfaceContext *c,int32_t xIn,int32_t yIn,int32_t mode)
{
    uint32_t dy=(c->geometry->diamond_y_min),dx=(c->geometry->diamond_x_min),x=(uint32_t)xIn,y=(uint32_t)yIn,cell,flags,Access,sprite;
    uint32_t left,top,columns=3,rows=3,width,height,endrow,endcol,col,row,bound,frame,image,variant,variation;
    if(!(uint8_t)pharaoh_map_grid_is_inside(c->geometry,(int32_t)(dx+x),(int32_t)(dy+y)))return;
    cell=(c->map->origin)+y*228u+x;(*c->current_tile)=cell;flags=c->map->terrain[cell];
    c->map->terrain[cell]=mode==1?flags|0x40000u:flags&0xfffbffffu;
    left=x-1u;top=y-1u;
    if((int32_t)left<0){columns=left+3u;left=0;}
    else{width=(c->map->width);if((int32_t)(left+3u)>(int32_t)width)columns=width-left;}
    if((int32_t)top<0){rows=top+3u;top=0;}
    else{height=(c->map->height);if((int32_t)(top+3u)>(int32_t)height)rows=height-top;}
    /* Native also pushes the unused top coordinate to the x-only water leaf. */
    bound=(uint8_t)pharaoh_map_sweep_has_passed(*c->sweep_countdown,*c->sweep_start,(int32_t)left);
    cell=(c->map->origin)+top*228u+left;endrow=top+rows;(*c->scan_y)=top;(*c->current_tile)=cell;
    if((int32_t)top>=(int32_t)endrow)return;
    endcol=left+columns;
    do{
        col=left;(*c->scan_x)=col;
        if((int32_t)col<(int32_t)endcol)do{
            flags=c->map->terrain[cell];
            if((flags&0x40000u)&&!(flags&8u)){
                pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,(int32_t)*c->current_tile,0x40000u);pharaoh_map_adjacent_flags_invert(c->adjacent);cell=(*c->current_tile);variation=c->random[cell];
                surface_match(c,c->water_context,48,(int32_t)variation,0);
                frame=(c->selection->frame);image=(bound?c->groups[0x2d1].first_image:c->groups[0x1fc].first_image);variant=(c->selection->orientation_offset);image+=frame+variant;cell=(*c->current_tile);
                c->image_ids[cell]=image;Access=c->property_flags[cell];sprite=c->edge_flags[cell];
                c->property_flags[cell]=(uint8_t)(Access&0xf0u);c->edge_flags[cell]=(uint8_t)(sprite|64u);
                pharaoh_map_set_vegetation_image(c,cell);cell=(*c->current_tile);
            }
            col=(*c->scan_x)+1u;++cell;(*c->scan_x)=col;(*c->current_tile)=cell;
        }while((int32_t)col<(int32_t)endcol);
        row=(*c->scan_y)+1u;(*c->scan_y)=row;cell+=228u-columns;row=(*c->scan_y);(*c->current_tile)=cell;
    }while((int32_t)row<(int32_t)endrow);
}

void pharaoh_map_refresh_marshland(PharaohMapSurfaceContext *c,int32_t leftIn,int32_t topIn,int32_t rightIn,int32_t bottomIn)
{
    uint32_t left=(uint32_t)leftIn,top=(uint32_t)topIn,right=(uint32_t)rightIn,bottom=(uint32_t)bottomIn,width,height,cell,stride,row,col,flags;
    if(leftIn<0)left=0;if(topIn<0)top=0;
    width=(c->map->width);if((int32_t)right>=(int32_t)width)right=width-1u;
    height=(c->map->height);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    cell=(c->map->origin);stride=width-right+left-1u;cell+=top*228u+left;row=top;
    if((int32_t)row>(int32_t)bottom)return;
    do{
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{flags=c->map->terrain[cell];if((flags&0x40000u)&&!(flags&8u))pharaoh_map_set_marshland_image(c,(int32_t)col,(int32_t)row,1);++cell;++col;}while((int32_t)col<=(int32_t)right);
        }
        cell+=(*c->border_size)+stride;++row;
    }while((int32_t)row<=(int32_t)bottom);
}
