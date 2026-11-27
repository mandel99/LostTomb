#include "pharaoh/map/construction_footprint.h"

_Static_assert(sizeof(void *)!=4 || offsetof(PharaohFloodTile,is_floodplain)==24,
    "native flood object flag is BYTE+18");
_Static_assert(sizeof(PharaohMapTileAreaOffset)==8 && sizeof(PharaohGatehouseImagePart)==16,
    "native square/gatehouse table strides");

/* Exact pinned native table data; use the same view across preview/placement. */
const PharaohConstructionFootprintTables pharaoh_construction_footprint_tables={
    .offsets_size2={0,1,228,229},
    .water_size2={{1,1,0,0},{0,1,0,1},{0,0,1,1},{1,0,1,0}},
    .water_borders={{{-1,2},{-227,457},{230,227},{-228,456}},{{-229,-226},{-226,458},{455,458},{-229,455}},{{-228,-227},{2,230},{456,457},{-1,227}}}
};

static void rotate_point(PharaohConstructionFootprintContext *c,
    uint32_t *x,uint32_t *y,uint32_t rotate,uint32_t distance)
{
    if(rotate==1u){
        uint32_t mode=(uint32_t)*c->surface->orientation;
        if(mode==6u)*y-=distance;
        else if(mode==4u){*x-=distance;*y-=distance;}
        else if(mode==2u)*x-=distance;
    }
}

/* Native0047D4D0: retain the actual8-byte square records and live controls.
 * The earlier scalar-offset helper remains available for its existing users. */
void pharaoh_construction_mark_current_footprint(PharaohConstructionFootprintContext *c)
{
    uint32_t rows=*c->rows,columns,base,i,j,tile;
    *c->placement_ok=1;if((int32_t)rows<=0)return;
    columns=*c->columns;base=*c->surface->current_tile;
    for(i=0;i<rows;++i)if((int32_t)columns>0)for(j=0;j<columns;++j){
        tile=base+c->surface->footprints[i][j].grid_offset;
        c->surface->property_flags[tile]|=16u;
    }
}

/* Complete typed reuse of MenuDragFootprint, native0047D360. The scalar
 * fixed switch suppresses rotation; native performs no indirect call. */
void pharaoh_construction_mark_footprint(PharaohConstructionFootprintContext *c,
    int32_t size_in,int32_t x_in,int32_t y_in,uint32_t mask,int32_t fixed,uint8_t allow)
{
    uint32_t size=(uint32_t)size_in,x=(uint32_t)x_in,y=(uint32_t)y_in;
    uint32_t base,i,j,tile,flags,seen_empty=0,seen_existing=0;
    *c->rows=size;*c->columns=size;
    if(!fixed)rotate_point(c,&x,&y,1,size-1u);
    if((int32_t)x<3||(int32_t)y<3)return;
    if((int32_t)(size+x-1u)>=(int32_t)((uint32_t)c->surface->map->width-3u))return;
    if((int32_t)(size+y-1u)>=(int32_t)((uint32_t)c->surface->map->height-3u))return;
    base=(uint32_t)c->surface->map->origin+y*228u+x;*c->surface->current_tile=base;
    if(size_in>0)for(i=0;i<size;++i)for(j=0;j<size;++j){
        tile=c->surface->footprints[i][j].grid_offset+base;
        flags=c->surface->map->terrain[tile];
        if(flags&0x10000u){if(!allow||seen_empty)return;seen_existing=1;}
        else{if(seen_existing)return;seen_empty=1;}
        if(flags&mask&0xeefed77fu)return;
        if(c->blocking_objects[tile])return;
    }
    pharaoh_construction_mark_current_footprint(c);
}

/* Complete typed reuse of MenuDragRegionGate, native0047D530; full EAX0/1. */
int32_t pharaoh_construction_footprint_is_free(PharaohConstructionFootprintContext *c,
    int32_t size_in,int32_t x_in,int32_t y_in,uint32_t mask,uint8_t allow)
{
    uint32_t size=(uint32_t)size_in,x=(uint32_t)x_in,y=(uint32_t)y_in;
    uint32_t row=0,col,columns=size,rows,tile,flags;
    *c->rows=size;*c->columns=size;
    rotate_point(c,&x,&y,1,size-1u);
    if((int32_t)x<0||(int32_t)y<0)return 0;
    if((int32_t)(size+x-1u)>=c->surface->map->width)return 0;
    if((int32_t)(size+y-1u)>=c->surface->map->height)return 0;
    *c->surface->current_tile=(uint32_t)c->surface->map->origin+y*228u+x;
    if(size_in>0)do{
        col=0;
        if((int32_t)columns>0)do{
            const PharaohFloodTile *flood;
            tile=c->surface->footprints[row][col].grid_offset+*c->surface->current_tile;
            if(pharaoh_map_grid_size_reaches_outside(c->surface->geometry,
                (int32_t)tile%228,(int32_t)tile/228,3))return 0;
            flood=c->surface->flood_tiles[tile];flags=c->surface->map->terrain[tile]&mask;
            if((flags&0x10000u)&&!allow)return 0;
            if(flags&0xeefed77fu)return 0;
            if(flood&&flood->is_floodplain==0u)return 0;
            columns=*c->columns;++col;
        }while((int32_t)col<(int32_t)columns);
        rows=*c->rows;++row;
    }while((int32_t)row<(int32_t)rows);
    return 1;
}

/* Complete typed reuse of MenuDragFootprint, native0047CC70. Flood/water
 * state persists across candidates; borders use the live orientation latch. */
int32_t pharaoh_map_water_determine_orientation_size2(PharaohConstructionFootprintContext *c,
    int32_t x_in,int32_t y_in,uint32_t rotate,uint8_t allow)
{
    uint32_t x=(uint32_t)x_in,y=(uint32_t)y_in,base,orient=0,state=0;
    uint32_t cost,matched,i,j,tile,required,flags,retain,latched,result;
    const PharaohFloodTile *flood;
    rotate_point(c,&x,&y,rotate,1);
    if((int32_t)x<0||(int32_t)y<0)return 999;
    if((int32_t)(x+1u)>=c->surface->map->width)return 999;
    if((int32_t)(y+1u)>=c->surface->map->height)return 999;
    *c->adjusted_orientation=0;base=(uint32_t)c->surface->map->origin+y*228u+x;
    do{
        cost=0;matched=0;
        for(i=0;i<4u;++i){
            tile=c->tables->offsets_size2[i]+base;
            required=c->tables->water_size2[orient][i];flags=c->surface->map->terrain[tile];
            if(required){
                flood=c->surface->flood_tiles[tile];retain=0;
                if(flood&&flood->is_floodplain==0u&&state!=2u){state=1;if(allow)retain=1;}
                if((flags&4u)&&state!=1u)state=2;else if(!retain)break;
                ++matched;if(flags&64u)++cost;if(flags&1u)++cost;
            }else{
                if(flags&0x10004u){
                    if(!allow)break;flood=c->surface->flood_tiles[tile];
                    if(!flood||flood->is_floodplain)break;
                }
                ++matched;if((flags&64u)||(flags&0x26341237u)||(flags&8u))++cost;
            }
        }
        latched=(uint32_t)*c->adjusted_orientation;
        for(i=0;i<2u;++i)for(j=0;j<3u;++j){
            tile=(uint32_t)c->tables->water_borders[j][latched][i]+base;
            flags=c->surface->map->terrain[tile];
            if(!(flags&(allow?0x10004u:4u))){
                if(!allow)matched=0;
                else{flood=c->surface->flood_tiles[tile];if(!flood||flood->is_floodplain)matched=0;}
            }
        }
        if(matched==4u){
            result=(uint32_t)*c->adjusted_orientation;*c->raw_orientation=(int32_t)latched;
            result-=(uint32_t)(*c->surface->orientation/2);
            *c->adjusted_orientation=(int32_t)result;
            if((int32_t)result<0)*c->adjusted_orientation=(int32_t)(result+4u);
            return (int32_t)cost;
        }
        orient=latched+1u;*c->adjusted_orientation=(int32_t)orient;
    }while((int32_t)orient<4);
    return 999;
}

/* Complete typed reuse of MenuDragPreviewGeometry, native004BFA50. */
void pharaoh_gatehouse_mark_construction(PharaohConstructionFootprintContext *c,
    int32_t x_in,int32_t y_in,uint32_t layout)
{
    uint32_t index;
    for(index=0;index<4u;++index){
        uint32_t part=c->temple_tables->gate_order[layout][index],x,y,orientation,size;
        const PharaohGatehouseImagePart *record=&c->temple_tables->gate_parts[part];
        if(layout==0u){x=(uint32_t)record->dx+(uint32_t)x_in;y=(uint32_t)record->dy+(uint32_t)y_in;}
        else if(layout==1u){x=(uint32_t)record->dy+(uint32_t)x_in;y=(uint32_t)record->dx+(uint32_t)y_in;}
        else{x=(uint32_t)x_in;y=(uint32_t)y_in;}
        orientation=(uint32_t)*c->surface->orientation;
        if(orientation!=0u){
            size=(uint32_t)record->size;
            if(orientation==2u)x+=size-1u;
            else if(orientation==4u){x+=size-1u;y+=size-1u;}
            else y+=size-1u;
        }
        size=(uint32_t)record->size;
        pharaoh_construction_mark_footprint(c,(int32_t)size,(int32_t)x,(int32_t)y,0xffffffbfu,0,0);
    }
}
