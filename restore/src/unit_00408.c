#include "dropdown_menu.h"
int32_t FUN_00425c40(int32_t x,int32_t y,RestoreSquareButton *buttons,int32_t count)
{
    uint32_t now=restore_dropdown_clock_ms();
    int tick=(int32_t)(now-DM_U(0x705690u))>=30;
    int32_t i;
    uintptr_t record=(uintptr_t)buttons;
    if(tick)DM_U(0x705690u)=now;
    for(i=0;i<count;++i,record+=24u){
        uint8_t delay=DM_B(record+13u);
        if(delay){
            delay=(uint8_t)(delay-1u);DM_B(record+13u)=delay;
            if(!delay){
                DM_B(record+12u)=1;DM_B(record+14u)=0;
                if(DM_B(record+15u)==5){
                    DM_U(0xd3b198u)=DM_U(record+16u);DM_U(0xd3b194u)=DM_U(record+20u);
                    restore_dropdown_callback(DM_U(record+8u));
                }
            }
        }else DM_B(record+14u)=0;
        if(DM_B(record+15u)==3)DM_B(record+12u)=0;
    }
    record=(uintptr_t)buttons;
    for(i=0;i<count;++i,record+=24u){
        int32_t left=dm_add(DM_S(record),x),top=dm_add(DM_S(record+2u),y);
        int32_t extent=DM_S(record+6u),mx=DM_I(0xe38e34u),my=DM_I(0xe38e30u);
        uint8_t kind;
        if(left>mx||dm_add(left,extent)<=mx||top>my||dm_add(top,extent)<=my)continue;
        if(DM_I(0x605a38u)>0&&DM_B(0xe38ddau)==1&&DM_U(0x605a3cu)==1){DM_U(0x7056b8u)=(uint32_t)i+1u;if(i>-1)DM_U(0x7056b8u)=21;}
        kind=DM_B(record+15u);
        if(kind==4){
            if(DM_B(0xe38e03u)||DM_B(0xe38e02u)){
                DM_B(record+12u)=1;DM_B(record+13u)=3;DM_B(record+14u)=0;
                DM_U(0xd3b198u)=DM_U(record+16u);DM_U(0xd3b194u)=DM_U(record+20u);DM_U(0xd3b190u)=0;
                restore_dropdown_callback(DM_U(record+8u));return i+1;
            }
            if(!DM_B(0xe38e00u))return 0;
            DM_B(record+13u)=3;
            if(!tick)return 0;
            {uint8_t repeat=(uint8_t)(DM_B(record+14u)+1u);DM_B(record+14u)=repeat;
             if(repeat>=48)DM_B(record+14u)=47;
             else if(repeat<8||!DM_B(0x5c9268u+repeat))return 0;}
            DM_U(0xd3b198u)=DM_U(record+16u);DM_U(0xd3b194u)=DM_U(record+20u);DM_U(0xd3b190u)=DM_B(record+14u);
            restore_dropdown_callback(DM_U(record+8u));return i+1;
        }
        if(kind==2){
            DM_B(record+13u)=1;
            if(!DM_B(record+14u)){
                DM_B(record+12u)^=1;
                DM_U(0xd3b198u)=DM_U(record+16u);DM_U(0xd3b194u)=DM_U(record+20u);
                restore_dropdown_callback(DM_U(record+8u));
            }
            return i+1;
        }
        if(kind==5){
            if(!DM_B(0xe38e03u)&&!DM_B(0xe38e02u))return 0;
            DM_B(record+13u)=10;DM_B(record+12u)=1;return i+1;
        }
    }
    return 0;
}
