#include "sun_temple.h"
#include "building_allocator.h"
#include "building_support.h"
#include "editor_brush_validation.h"
#include "placement_palette.h"
#include "road_place.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((uint32_t)(int32_t)(int16_t)W(a))

void restore_sun_temple_map(uint32_t building)
{
    uint32_t flags,image=restore_placement_image_005414b0((int32_t)building,(uint32_t)(uintptr_t)&flags);
    uint32_t r=building*264u,piece=H(0x7999eau+r),size=U(0x601de0u+piece*52u);
    uint32_t row,column,row_table=0x5cc8c0u,cell;
    if(!(flags&1u)){
        FUN_0047bf50((uint16_t)building,(int32_t)H(0x7999deu+r),(int32_t)H(0x7999e0u+r),
                     (int32_t)size,(int32_t)size,image,piece?0x10000008u:8u);
        return;
    }
    U(0xbc9768u)=U(0xbca2d8u)+H(0x7999deu+r)+H(0x7999e0u+r)*228u;
    if((int32_t)size<=0)return;
    for(row=0;row<size;++row,row_table+=48u){
        for(column=0;column<size;++column){
            /* Origin and the ground-image tag can change through raster aliases. */
            cell=U(row_table+column*8u)+U(0xbc9768u);
            U(0xaf598cu+cell*4u)|=0x10000000u;
            W(0xb4e6fcu+cell*2u)=(uint16_t)building;
            B(0xb285ccu+cell)&=0xe0u;B(0xb67d1cu+cell)|=0x40u;
            if((flags&2u)&&!((column==0u||column==size-1u)&&
                              (row_table==0x5cc8c0u||row==size-1u)))continue;
            if(image)U(0xb7482cu+cell*4u)=image==U(0xf56b54u)?image+(B(0xacf85cu+cell)&7u):image;
        }
    }
}
void restore_sun_temple_create(int32_t x,int32_t y,uint32_t root)
{
    uint32_t registration=(uint32_t)FUN_004f4f80((int32_t)root),r=root*264u;
    uint32_t dx=U(0x601dd8u),dy=U(0x601ddcu),building,previous=root,record,index,cell;
    uint8_t size;
    W(0x7999deu+r)=(uint16_t)((uint32_t)x+dx);
    W(0x7999e0u+r)=(uint16_t)((uint32_t)y+dy);
    cell=U(0xbca2d8u)+(dy+(uint32_t)y)*228u+dx+(uint32_t)x;size=B(0x601de0u);
    U(0x7999e4u+r)=cell;W(0x7999eau+r)=0;B(0x7999dbu+r)=size;
    W(0x799a10u+r)=0;U(0x799a2cu+r)=registration;
    U(0x799a30u+r)=0;U(0x799a38u+r)=1;U(0x799a34u+r)=0;
    restore_sun_temple_map(root);(void)FUN_00485100((int32_t)root);
    for(record=0x601e10u,index=1u;record<0x60322cu;record+=52u,++index){
        dy=U(record);dx=U(record-4u);
        building=(uint32_t)restore_building_allocate_0046a400(U(0xeaac28u),0xd5u,
                       (int32_t)(dx+(uint32_t)x),(int32_t)(dy+(uint32_t)y),0,(int32_t)(previous+1u));
        r=building*264u;W(0x799a10u+r)=(uint16_t)previous;
        W(0x799a12u+previous*264u)=(uint16_t)building;previous=building;
        size=B(record+4u);W(0x799a12u+r)=0;W(0x7999eau+r)=(uint16_t)index;
        B(0x7999dbu+r)=size;U(0x799a2cu+r)=registration;
        U(0x799a30u+r)=0;U(0x799a38u+r)=0;U(0x799a34u+r)=0;
        restore_sun_temple_map(building);(void)FUN_00485100((int32_t)building);
    }
}
void restore_sun_temple_frame(uint32_t target,struct restore_menu_input_frame *f)
{
    uint32_t sp=f->r[4];
    switch(target){
    case 0x5417b0u:restore_sun_temple_create((int32_t)U(sp+4u),(int32_t)U(sp+8u),U(sp+12u));break;
    case 0x541920u:restore_sun_temple_map(U(sp+4u));break;
    default:abort();
    }
    f->r[4]=sp+4u;
}
