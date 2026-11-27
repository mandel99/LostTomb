#include "pharaoh/map/native_leaf_queries.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohBuildingTypeRecord)==264,"native building stride");
_Static_assert(offsetof(PharaohBuildingTypeRecord,reserved_3e)+1==0x3f,"well access BYTE");

/* Complete current/old Unit01135 C with actual native load timing. */
void pharaoh_map_update_orientation_corners(const PharaohNativeMapLeafContext *c)
{
    int32_t height=*c->height;
    if(height<=0)return;
    const int32_t rotation=*c->corner_rotation;
    uint32_t row=0;
    do{
        const int32_t left=c->row_left[row],right=c->row_right[row];
        uint32_t tile=(uint32_t)c->row_tiles[row];
        if(left<right){
            uint32_t remaining=(uint32_t)right-(uint32_t)left;
            do{
                const uint8_t kind=c->bitfields[tile]&15u;
                uint8_t marker=c->markers[tile];
                const uint8_t position=marker&63u;
                if(kind==0)c->markers[tile]=(uint8_t)(marker|64u);
                else{
                    marker&=0xbfu;c->markers[tile]=marker;
                    if(kind<=5){
                        const int match=(rotation==0&&position==kind*8u)||
                            (rotation==6&&position==kind*9u)||
                            (rotation==4&&position==kind)||(rotation==2&&position==0);
                        if(match)c->markers[tile]=(uint8_t)(marker|64u);
                    }
                }
                ++tile;
            }while(--remaining);
        }
        height=*c->height;++row;
    }while((int32_t)row<height);
}

void pharaoh_map_rotate_bridge_sprites(const PharaohNativeMapLeafContext *c)
{
    const int32_t height=*c->height;
    if(height<=0)return;
    const uint32_t turn=*c->bridge_turn;
    for(int32_t row=0;row<height;++row){
        const int32_t left=c->row_left[row],right=c->row_right[row];
        uint32_t tile=(uint32_t)c->row_tiles[row];
        if(left<right){
            uint32_t remaining=(uint32_t)right-(uint32_t)left;
            do{
                /* Native tests BYTE at a DWORD terrain-pool unit. */
                if((*(const volatile uint8_t *)&c->terrain[tile]&4u)!=0){
                    uint8_t direction=c->bridge_sprites[tile];
                    switch(direction){
                    case 1:direction=(uint8_t)(turn==1?6:4);break;
                    case 2:direction=5;break;
                    case 3:direction=(uint8_t)(turn==1?4:6);break;
                    case 4:direction=(uint8_t)(turn==1?1:3);break;
                    case 5:direction=2;break;
                    case 6:direction=(uint8_t)(turn==1?3:1);break;
                    default:goto next_bridge;
                    }
                    c->bridge_sprites[tile]=direction;
                }
next_bridge:++tile;
            }while(--remaining);
        }
    }
}

/* Whole Unit01167 C. The native flag is BYTE3F, and no cursor is published. */
void pharaoh_map_mark_well_access(const PharaohNativeMapLeafContext *c,int32_t id,int32_t radius)
{
    const int32_t x=c->buildings[id].x,y=c->buildings[id].y;
    uint32_t left=(uint32_t)x-(uint32_t)radius,right=(uint32_t)x+(uint32_t)radius;
    uint32_t top=(uint32_t)y-(uint32_t)radius,bottom=(uint32_t)y+(uint32_t)radius;
    *c->result_y=top;
    if((int32_t)left<0)left=0;if((int32_t)top<0){top=0;*c->result_y=top;}
    const uint32_t width=(uint32_t)*c->width,height=(uint32_t)*c->height;
    if((int32_t)right>=(int32_t)width)right=width-1u;
    if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    uint32_t tile=*c->origin+top*228u+left;
    if((int32_t)top<=(int32_t)bottom){
        uint32_t rows=bottom-top+1u;*c->result_y=top+rows;
        do{
            *c->result_x=left;
            if((int32_t)left<=(int32_t)right){
                uint32_t scan=tile,cells=right-left+1u;tile+=cells;*c->result_x=left+cells;
                do{
                    const int32_t occupant=c->building_grid[scan++];
                    if(occupant!=0)c->buildings[occupant].reserved_3e[1]=1;
                }while(--cells);
            }
            tile+=left-right+227u;
        }while(--rows);
    }
}

/* Whole current/old Unit01207/1208 rectangle C; cursor stays live per cell. */
static void native_land_change(const PharaohNativeMapLeafContext *c,int32_t x,int32_t y,
    int32_t size,int32_t radius,int mark)
{
    uint32_t left=(uint32_t)x-(uint32_t)radius;
    uint32_t right=(uint32_t)x+(uint32_t)size-1u+(uint32_t)radius;
    uint32_t top=(uint32_t)y-(uint32_t)radius;
    uint32_t bottom=(uint32_t)y+(uint32_t)size-1u+(uint32_t)radius;
    *c->result_y=top;
    if((int32_t)left<0)left=0;if((int32_t)top<0){top=0;*c->result_y=top;}
    const uint32_t width=(uint32_t)*c->width,height=(uint32_t)*c->height;
    if((int32_t)right>=(int32_t)width)right=width-1u;
    if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    *c->cursor=*c->origin+top*228u+left;
    if((int32_t)top<=(int32_t)bottom){
        uint32_t rows=bottom-top+1u;*c->result_y=top+rows;
        do{
            *c->result_x=left;
            if((int32_t)left<=(int32_t)right){
                uint32_t cells=right-left+1u;*c->result_x=left+cells;
                do{
                    const uint32_t tile=*c->cursor;
                    if(mark)c->markers[tile]=(uint8_t)(c->markers[tile]|0x80u);
                    else c->markers[tile]=(uint8_t)(c->markers[tile]&0x7fu);
                    *c->cursor=*c->cursor+1u;
                }while(--cells);
            }
            *c->cursor=*c->cursor+left-right+227u;
        }while(--rows);
    }
}

void pharaoh_map_native_land_mark(const PharaohNativeMapLeafContext *c,int32_t x,int32_t y,int32_t size,int32_t radius)
{native_land_change(c,x,y,size,radius,1);}
void pharaoh_map_native_land_unmark(const PharaohNativeMapLeafContext *c,int32_t x,int32_t y,int32_t size,int32_t radius)
{native_land_change(c,x,y,size,radius,0);}
