#include "placement_preview.h"
#include "city_footprint_validation.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole4DEBE0. The geometry copier writes one 52-byte record, including
   size and four x/y screen offsets. These are fields of the same record,
   not independent stack locals inferred from the original stack layout. */
void FUN_004debe0(void)
{
    int32_t record[13],screen_x=0,screen_y=0,index,count,draw;
    uint32_t tint,rotation,horizontal,vertical,x,y,size,turn;
    tint=FUN_004def00(I(U(0xbca300u)))?0x1fe3u:0xf863u;
    rotation=(uint32_t)(I(U(0xbca2e0u))/2);
    FUN_004f0ba0(I(U(0xbca304u)),I(U(0xbca308u)),&screen_x,&screen_y);
    horizontal=U(0x70e45cu)+1u;vertical=U(0x70e44cu);
    for(index=0;index<116;++index){
        turn=U(0xbca2e0u);
        restore_city_geometry_004ded50(record,index,I(turn),I(turn));
        x=(uint32_t)record[5u+rotation]+(uint32_t)screen_x;
        y=(uint32_t)record[9u+rotation]+U(0x70e44cu)+(uint32_t)screen_y;
        size=(uint32_t)record[2];count=I(U(0x5cfcd4u+size*4u));
        turn=U(0xbca2e0u);
        if(I(size)>1){
            if(turn==2u){x+=(size-1u)*horizontal;y-=(size-1u)*vertical;}
            else if(turn==4u)y-=(size-1u)*vertical*2u;
            else if(turn==6u){x-=(size-1u)*horizontal;y-=(size-1u)*vertical;}
        }
        for(draw=0;draw<count;++draw){
            uint32_t offset=(uint32_t)draw*8u;
            (void)FUN_0053f4f0(I(U(0xf56be4u)),I(U(0x5cfcf0u+offset)+x),
                I(U(0x5cfcf4u+offset)-U(0x70e44cu)+y),tint);
        }
    }
}
