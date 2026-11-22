#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/map/surface_queries.h"
#include "pharaoh/runtime/map_ring.h"
#include "pharaoh/core/x87_numeric.h"

static const double meadow_first_scale=0.010101010101010102;
static const double meadow_second_scale=3.0;
static const double fertility_divisor=96.0,fertility_scale=12.0;
static uint32_t group_image(const PharaohMapSurfaceContext *c,uint32_t group)
{return c->groups[group].first_image;}

/* Native00481490. Ring0 does not update the shared scan tile. */
uint8_t pharaoh_map_ring_is_wooded(PharaohMapSurfaceContext *c,
    int32_t x,int32_t y,int32_t ring)
{
    if(ring<=0)return 1;
    const int32_t level=ring>=11?11:ring;
    const int32_t count=level*8,start=(level-1)*level*4;
    *c->current_tile=(uint32_t)c->map->origin+(uint32_t)y*228u+(uint32_t)x;
    for(int32_t i=0;i<count;++i){
        if(!pharaoh_map_ring_point_is_outside(c->rings,c->map->width,c->map->height,
            x,y,1,level,i)){
            const uint32_t tile=*c->current_tile+(uint32_t)c->distance_offsets[start+i];
            if(!(c->map->terrain[tile]&3u))return 0;
        }
    }
    return 1;
}

/* Native00486600. */
void pharaoh_map_set_vegetation_image(PharaohMapSurfaceContext *c,uint32_t tile)
{
    const uint32_t flags=c->map->terrain[tile];
    uint32_t group,variant,band;
    if((flags&1u) && !(flags&0x80000u)){
        uint32_t fertility=c->fertility[tile];
        if(fertility>100u)fertility=100u;
        band=fertility/34u;
        if(c->variation_tag[tile]!=255u){
            c->image_ids[tile]=group_image(c,0x1fdu)+(c->random[tile]&7u)+band*8u;
            return;
        }
        const int32_t relative=(int32_t)(tile-(uint32_t)c->map->origin);
        const int32_t x=relative%228,y=relative/228;
        int32_t ring;
        for(ring=3;;--ring)if(pharaoh_map_ring_is_wooded(c,x,y,ring) || !ring)break;
        if(ring<2)band<<=4;else{band=48;ring-=2;}
        c->image_ids[tile]=group_image(c,0x1f5u)+(c->random[tile]&7u)+(uint32_t)ring*8u+band;
        return;
    }
    if(!(flags&0x40000u) || (flags&0x80000u) ||
        pharaoh_map_terrain_count_neighbours_with(c->map->terrain,(int32_t)tile,0x40000u)!=8u)return;
    const int32_t relative=(int32_t)(tile-(uint32_t)c->map->origin);
    const uint8_t bound=pharaoh_map_sweep_has_passed(*c->sweep_countdown,*c->sweep_start,relative%228);
    if(c->variation_tag[tile]==255u)group=bound?0x2d2u:0x211u;
    else group=bound?0x2d1u:0x1fcu;
    variant=c->random[tile]&7u;
    c->image_ids[tile]=group_image(c,group)+variant;
}

/* Shared clipping arithmetic of00479770/00479B70, without changing their
 * different observable writes to scan_x/scan_y/current_tile. */
static void clip_neighborhood(const PharaohMapSurfaceContext *c,int32_t x,int32_t y,
    int32_t *left,int32_t *top,int32_t *cols,int32_t *rows)
{
    *left=(int32_t)((uint32_t)x-1u);*top=(int32_t)((uint32_t)y-1u);*cols=3;*rows=3;
    if(*left<0){*cols=(int32_t)((uint32_t)*left+3u);*left=0;}
    else if((int32_t)((uint32_t)*left+3u)>c->map->width)*cols=c->map->width-*left;
    if(*top<0){*rows=(int32_t)((uint32_t)*top+3u);*top=0;}
    else if((int32_t)((uint32_t)*top+3u)>c->map->height)*rows=c->map->height-*top;
}

void pharaoh_map_set_tree_image(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t set)
{
    if(!pharaoh_map_grid_is_inside(c->geometry,
        (int32_t)((uint32_t)c->geometry->diamond_x_min+(uint32_t)x),
        (int32_t)((uint32_t)c->geometry->diamond_y_min+(uint32_t)y)))return;
    uint32_t tile=(uint32_t)c->map->origin+(uint32_t)y*228u+(uint32_t)x;
    c->map->terrain[tile]=set==1?c->map->terrain[tile]|1u:c->map->terrain[tile]&0xfffffffeu;
    int32_t left,top,cols,rows;
    clip_neighborhood(c,x,y,&left,&top,&cols,&rows);
    const int32_t endrow=top+rows,endcol=left+cols;
    *c->scan_y=top;
    tile=(uint32_t)c->map->origin+(uint32_t)top*228u+(uint32_t)left;*c->current_tile=tile;
    if(top>=endrow)return;
    do{
        *c->scan_x=left;
        if(left<endcol)do{
            const uint32_t flags=c->map->terrain[tile];
            if((flags&1u) && !(flags&0x600u)){
                pharaoh_map_set_vegetation_image(c,tile);tile=*c->current_tile;
                c->property_flags[tile]&=0xf0u;c->edge_flags[tile]|=0x40u;c->canal_water[tile]=0;
            }
            ++*c->scan_x;++tile;*c->current_tile=tile;
        }while(*c->scan_x<endcol);
        ++*c->scan_y;tile+=228u-(uint32_t)cols;*c->current_tile=tile;
    }while(*c->scan_y<endrow);
}

void pharaoh_map_set_meadow_image(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t set)
{
    if(!pharaoh_map_grid_is_inside(c->geometry,
        (int32_t)((uint32_t)c->geometry->diamond_x_min+(uint32_t)x),
        (int32_t)((uint32_t)c->geometry->diamond_y_min+(uint32_t)y)))return;
    uint32_t tile=(uint32_t)c->map->origin+(uint32_t)y*228u+(uint32_t)x;
    c->map->terrain[tile]=set==1?c->map->terrain[tile]|0x800u:c->map->terrain[tile]&0xfffff7ffu;
    int32_t left,top,cols,rows;clip_neighborhood(c,x,y,&left,&top,&cols,&rows);
    const int32_t endrow=top+rows,endcol=left+cols;*c->scan_y=top;
    tile=(uint32_t)c->map->origin+(uint32_t)top*228u+(uint32_t)left;*c->current_tile=tile;
    if(top>=endrow)return;
    *c->scan_y=endrow;
    for(int32_t row=top;row<endrow;++row){
        *c->scan_x=left;
        if(left<endcol){
            *c->scan_x=endcol;
            for(int32_t col=left;col<endcol;++col,++tile){
                const uint32_t flags=c->map->terrain[tile];
                if(!(flags&0x800u) || (flags&0x700u) || (flags&0x40u) ||
                    ((flags&8u) && !(flags&0x10000000u)) || (flags&0x20u) || (flags&0x1000u))continue;
                uint32_t first=(uint32_t)pharaoh_x87_i32_multiply_pair((int32_t)c->meadow_value[tile]-1,
                    &meadow_first_scale,&meadow_second_scale);
                uint32_t fertility=c->fertility[tile];if(fertility>100u)fertility=100u;
                uint32_t second=(uint32_t)pharaoh_x87_i32_divide_multiply((int32_t)fertility-1,
                    &fertility_divisor,&fertility_scale);
                uint32_t group=0x216u;
                if(!first && !second){group=0x233u;second=c->random[tile]&7u;}
                else if(first==2u && !second){group=0x228u;first=0;second=c->random[tile]&7u;}
                else if(first==2u && second==11u){group=0x227u;first=0;second=c->random[tile]&7u;}
                c->image_ids[tile]=group_image(c,group)+first*12u+second;
                c->property_flags[tile]&=0xf0u;c->edge_flags[tile]|=0x40u;c->canal_water[tile]=0;
            }
        }
        tile+=228u-(uint32_t)cols;
    }
    *c->current_tile=tile;
}

void pharaoh_map_refresh_trees(PharaohMapSurfaceContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
    if(x0<0)x0=0;if(y0<0)y0=0;
    if(x1>=c->map->width)x1=c->map->width-1;if(y1>=c->map->height)y1=c->map->height-1;
    const uint32_t step=(uint32_t)c->map->width-(uint32_t)x1+(uint32_t)x0-1u;
    uint32_t tile=(uint32_t)c->map->origin+(uint32_t)y0*228u+(uint32_t)x0;
    for(int32_t y=y0;y<=y1;++y){
        for(int32_t x=x0;x<=x1;++x,++tile){
            const uint32_t flags=c->map->terrain[tile];
            if((flags&1u) && !(flags&0x600u))pharaoh_map_set_tree_image(c,x,y,1);
        }
        tile+=(uint32_t)*c->border_size+step;
    }
}

void pharaoh_map_refresh_meadow(PharaohMapSurfaceContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
    if(x0<0)x0=0;if(y0<0)y0=0;
    if(x1>=c->map->width)x1=c->map->width-1;if(y1>=c->map->height)y1=c->map->height-1;
    const uint32_t step=(uint32_t)c->map->width-(uint32_t)x1+(uint32_t)x0-1u;
    uint32_t tile=(uint32_t)c->map->origin+(uint32_t)y0*228u+(uint32_t)x0;
    for(int32_t y=y0;y<=y1;++y){
        for(int32_t x=x0;x<=x1;++x,++tile){
            const uint32_t flags=c->map->terrain[tile];
            if((flags&0x800u) && !(flags&0x1700u) && !(flags&0x40u) &&
                (!(flags&8u) || (flags&0x10000000u)) && !(flags&0x20u))
                pharaoh_map_set_meadow_image(c,x,y,1);
        }
        tile+=(uint32_t)*c->border_size+step;
    }
}
