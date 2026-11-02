#include "dropdown_menu.h"
extern uint32_t FUN_004d5ef0(uint32_t group);
extern void FUN_00536990(int32_t x,int32_t y,int32_t width,int32_t height);
extern void FUN_004ccbb0(int32_t group,int32_t entry,int32_t x,int32_t y,uint32_t font,uint32_t color);
void FUN_00425430(void)
{
    uintptr_t bar;int32_t count,left,top,group,width,i;
    if(DM_B(0xe38ddau)!=3||!DM_U(0x705730u))return;
    bar=DM_U(0x705724u);DM_U(0x705720u)=DM_U(bar+8u);
    count=DM_S(bar+12u);left=DM_S(bar);top=DM_S(bar+4u);group=DM_S(bar+6u);
    DM_W(0xd3b5bcu)=(uint16_t)((uint32_t)count*20u+20u);DM_W(0xd3b5beu)=10;
    width=(int32_t)FUN_004d5ef0((uint32_t)group);DM_W(0xd3b5beu)=(uint16_t)width;
    FUN_00536990(left,dm_add(top,18),(int16_t)width,DM_S(0xd3b5bcu)/16);
    for(i=1;i<=count;++i){
        uintptr_t item=DM_U(0x705720u);int32_t entry=DM_S(item+2u),y=dm_add(dm_add(DM_S(item),top),30);
        if((uint32_t)i==DM_U(0x70572cu)){
            uint32_t font=DM_U(0xf56fa4u);
            DM_U(0xd3b510u)=1;DM_W(0xd3b5beu)=(uint16_t)width;DM_W(0xd3b5bcu)=16;
            FUN_004ccbb0(group,entry,dm_add(left,8),y,font,0xfac1);DM_U(0xd3b510u)=0;
        }else FUN_004ccbb0(group,entry,dm_add(left,8),y,DM_U(0xf56f8cu),0);
        DM_U(0x705720u)=DM_U(0x705720u)+16u;
    }
}
