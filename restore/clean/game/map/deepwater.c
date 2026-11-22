#include "surface_refresh_internal.h"

void pharaoh_map_shrink_deepwater_around(PharaohMapSurfaceContext *c,int32_t x,int32_t y)
{
    uint32_t xmin=(uint32_t)x-1u,xmax,width,ymin=(uint32_t)y-1u,ymax,height,padding,step,tile,row,column,flags,remove;
    width=(c->map->width);
    if((int32_t)xmin<0)xmin=0;xmax=(uint32_t)x+1u;if((int32_t)width<(int32_t)xmax)xmax=width;
    if((int32_t)ymin<0)ymin=0;height=(c->map->height);ymax=(uint32_t)y+1u;if((int32_t)height<(int32_t)ymax)ymax=height;
    padding=(*c->border_size);step=padding-xmax+width+xmin-1u;tile=(c->map->origin)+ymin*228u+xmin;row=ymin;
    if((int32_t)row<=(int32_t)ymax){do{
        column=xmin;
        if((int32_t)xmin<=(int32_t)xmax){do{
            flags=c->map->terrain[(tile)];
            if(flags&0x4000000u){
                remove=!pharaoh_map_terrain_column_has_two(c->map,(int32_t)column,(int32_t)row,0x4000000u);
                if(!remove)remove=!pharaoh_map_terrain_row_has_two(c->map,(int32_t)column,(int32_t)row,0x4000000u);
                if(!remove){((*c->current_tile) = (tile));pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,(int32_t)*c->current_tile,0x4000000u);remove=surface_deepwater_has_two_run(c->adjacent);}
                if(remove){flags=c->map->terrain[(tile)];(c->map->terrain[(tile)] = (flags&0xfbffffffu));pharaoh_map_shrink_deepwater_around(c,(int32_t)column,(int32_t)row);}
            }
            ++column;++tile;
        }while((int32_t)column<=(int32_t)xmax);}
        ++row;tile+=step;
    }while((int32_t)row<=(int32_t)ymax);}

}

void pharaoh_map_update_deepwater(PharaohMapSurfaceContext *c,int32_t xmin,int32_t ymin,int32_t xmax,int32_t ymax)
{
    uint32_t width,height,base,saved_tile,step,row_offset,tile,row,column,flags,clear;
    uint32_t outer_tile,next_tile,xlo,xhi,ylo,yhi,substep,subtile,subrow,subcolumn,padding,remove;
    if(xmin<0)xmin=0;if(ymin<0)ymin=0;
    width=(c->map->width);if(xmax>=(int32_t)width)xmax=(int32_t)(width-1u);
    height=(c->map->height);if(ymax>=(int32_t)height)ymax=(int32_t)(height-1u);
    base=(c->map->origin);step=width-(uint32_t)xmax+(uint32_t)xmin-1u;saved_tile=(*c->current_tile);row_offset=(uint32_t)ymin*228u;tile=base+row_offset+(uint32_t)xmin;row=(uint32_t)ymin;
    if(ymin<=ymax){do{
        if(xmin<=xmax){
            column=(uint32_t)xmin;
            do{
                flags=c->map->terrain[(tile)];
                if((flags&0x80004u)==4u){
                    clear=pharaoh_map_terrain_any_tile_in_square_without(c->map,(int32_t)column,(int32_t)row,2,4);
                    if(!clear)clear=pharaoh_map_terrain_any_tile_in_square_with(c->map,(int32_t)column,(int32_t)row,2,0x10000u);
                    flags=c->map->terrain[(tile)];(c->map->terrain[(tile)] = (clear?flags&0xfbffffffu:flags|0x4000000u));
                }
                ++column;++tile;
            }while((int32_t)column<=xmax);
            width=(c->map->width);
        }
        padding=(*c->border_size);tile+=padding+step;++row;
    }while((int32_t)row<=ymax);}
    base=(c->map->origin);tile=base+row_offset+(uint32_t)xmin;row=(uint32_t)ymin;
    if(ymin<=ymax){do{
        if(xmin<=xmax){
            outer_tile=tile;next_tile=tile+(uint32_t)xmax-(uint32_t)xmin+1u;column=(uint32_t)xmin;
            do{
                flags=c->map->terrain[(outer_tile)];
                if(flags&0x4000000u){
                    xlo=column-1u;if((int32_t)xlo<0)xlo=0;xhi=column+1u;if((int32_t)width<(int32_t)xhi)xhi=width;
                    height=(c->map->height);ylo=row-1u;if((int32_t)ylo<0)ylo=0;yhi=row+1u;if((int32_t)height<(int32_t)yhi)yhi=height;
                    padding=(*c->border_size);substep=padding-xhi+width+xlo-1u;base=(c->map->origin);subtile=base+ylo*228u+xlo;subrow=ylo;
                    if((int32_t)ylo<=(int32_t)yhi){do{
                        if((int32_t)xlo<=(int32_t)xhi){
                            subcolumn=xlo;
                            do{
                                flags=c->map->terrain[(subtile)];
                                if(flags&0x4000000u){
                                    remove=!pharaoh_map_terrain_column_has_two(c->map,(int32_t)subcolumn,(int32_t)subrow,0x4000000u);
                                    if(!remove)remove=!pharaoh_map_terrain_row_has_two(c->map,(int32_t)subcolumn,(int32_t)subrow,0x4000000u);
                                    if(!remove){((*c->current_tile) = (subtile));pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,(int32_t)*c->current_tile,0x4000000u);remove=surface_deepwater_has_two_run(c->adjacent);}
                                    if(remove){flags=c->map->terrain[(subtile)];(c->map->terrain[(subtile)] = (flags&0xfbffffffu));pharaoh_map_shrink_deepwater_around(c,(int32_t)subcolumn,(int32_t)subrow);}
                                }
                                ++subcolumn;++subtile;
                            }while((int32_t)subcolumn<=(int32_t)xhi);
                            width=(c->map->width);
                        }
                        subtile+=substep;++subrow;
                    }while((int32_t)subrow<=(int32_t)yhi);}
                }
                ++column;++outer_tile;
            }while((int32_t)column<=xmax);
            tile=next_tile;
        }
        padding=(*c->border_size);tile+=padding+step;++row;
    }while((int32_t)row<=ymax);}
    ((*c->current_tile) = (saved_tile));
}
