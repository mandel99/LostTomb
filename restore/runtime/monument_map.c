#include "monument_map.h"
#include "placement_queries.h"
#include "road_place.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((uint32_t)(int32_t)(int16_t)W(a))
static void square(uint32_t building,uint32_t r,uint32_t size,uint32_t image)
{FUN_0047bf50((uint16_t)building,(int32_t)H(0x7999deu+r),(int32_t)H(0x7999e0u+r),(int32_t)size,(int32_t)size,image,8);}
void restore_monument_map_004017a0(uint32_t self,uint32_t building)
{
    uint32_t flags,image=restore_placement_image_query(self,(int32_t)building,(uint32_t)(uintptr_t)&flags);
    uint32_t r=building*264u,index=H(0x7999eau+r),size=U(self+0x54u+index*52u);
    square(building,r,size,image);
}
void restore_monument_map_004e69c0(uint32_t self,uint32_t building)
{
    uint32_t flags,image=restore_placement_image_query(self,(int32_t)building,(uint32_t)(uintptr_t)&flags);
    uint32_t r=building*264u,index=H(0x7999eau+r),descriptor=self+index*52u;
    uint32_t special=U(descriptor+0x58u),size=U(descriptor+0x54u),cell,old_flags;
    if(special!=1){square(building,r,size,image);return;}
    cell=U(0xbca2d8u)+H(0x7999deu+r)+H(0x7999e0u+r)*228u;
    old_flags=U(0xaf598cu+cell*4u);
    W(0xb4e6fcu+cell*2u)=(uint16_t)building;
    U(0xaf598cu+cell*4u)=old_flags|0x10000000u;
}
static int image_corner(uint32_t index,uint32_t row_table,uint32_t column)
{
    switch(index){
    case 13:return column==0u&&(int32_t)row_table>=0x5cc920;
    case 14:return column>=2u&&(int32_t)row_table>=0x5cc920;
    case 15:return column==0u&&(int32_t)row_table<=0x5cc8c0;
    case 16:return column>=2u&&(int32_t)row_table<=0x5cc8c0;
    default:return 1;
    }
}
void restore_monument_map_0040dca0(uint32_t self,uint32_t building)
{
    uint32_t flags,image=restore_placement_image_query(self,(int32_t)building,(uint32_t)(uintptr_t)&flags);
    uint32_t r=building*264u,index=H(0x7999eau+r),root=building-index;
    uint32_t size=U(self+0x54u+index*52u),row,column,row_table=0x5cc8c0u,stage,cell;
    if(!(flags&1u)){square(building,r,size,image);return;}
    U(0xbc9768u)=U(0xbca2d8u)+H(0x7999deu+r)+H(0x7999e0u+r)*228u;
    if((int32_t)size<=0)return;
    for(row=0;row<size;++row,row_table+=48u){
        /* The original latches stage once per row and reloads origin per cell. */
        stage=U(0x799a30u+root*264u);
        for(column=0;column<size;++column){
            cell=U(row_table+column*8u)+U(0xbc9768u);
            if((int32_t)stage<3)B(0xaf598fu+cell*4u)|=0x10u;
            else U(0xaf598cu+cell*4u)=8;
            B(0xb285ccu+cell)&=0xe0u;B(0xb67d1cu+cell)|=0x40u;
            W(0xb4e6fcu+cell*2u)=(uint16_t)building;
            if((flags&2u)&&image==U(0xf573bcu)&&!image_corner(index,row_table,column))continue;
            if(image)U(0xb7482cu+cell*4u)=image==U(0xf56b54u)?image+(B(0xacf85cu+cell)&7u):image;
        }
    }
}
void restore_monument_map_frame(uint32_t target,struct restore_menu_input_frame *f)
{
    uint32_t sp=f->r[4],self=f->r[1],building=U(sp+4u);
    switch(target){
    case 0x4017a0u:restore_monument_map_004017a0(self,building);break;
    case 0x40dca0u:restore_monument_map_0040dca0(self,building);break;
    case 0x4e69c0u:restore_monument_map_004e69c0(self,building);break;
    default:abort();
    }
    f->r[4]=sp+8u;
}
