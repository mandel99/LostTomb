#include "actor_callbacks_small.h"
extern void FUN_004b17d0(int32_t actor),FUN_004b1d50(int32_t actor),FUN_004b1ef0(int32_t actor,int32_t flag),FUN_004b3250(int32_t actor);
extern void FUN_004a5da0(int32_t actor,uint32_t speed);
extern uint32_t FUN_0047fe60(int32_t x,int32_t y,uint32_t size,int32_t radius);
__declspec(noinline) void FUN_00493610(void)
{
    uint32_t id=AS_U(0x00d056f8u),a=as_actor(id),kind=AS_B(a+2u),speed,b;
    uint8_t state=AS_B(a+0x30u);int32_t home;
    speed=AS_U(0x007090ecu+kind*56u);home=AS_S(a+0x52u);
    if(state==5){FUN_004b17d0((int32_t)id);return;}
    if(state==4){FUN_004b1d50((int32_t)id);return;}
    if(state==1){
        AS_B(a+0x5du)=0;AS_W(a+0x3cu)=(uint16_t)(AS_W(a+0x3cu)+1u);
        if(AS_S(a+0x3cu)>=AS_S(a+0x3au)){
            uint32_t size,found;int32_t y,x;uint16_t target_x;
            b=as_building(home);size=AS_B(b+3u);y=AS_S(b+8u);x=AS_S(b+6u);
            found=FUN_0047fe60(x,y,size,2);id=AS_U(0x00d056f8u);a=as_actor(id);
            if(!found){AS_B(a+6u)=2;FUN_004b1ef0((int32_t)id,1);return;}
            target_x=AS_W(0x00d3b4e4u);AS_B(a+0x30u)=2;
            AS_W(a+0x1cu)=target_x;AS_W(a+0x1eu)=AS_W(0x00d3b4e0u);
            FUN_004b3250((int32_t)id);id=AS_U(0x00d056f8u);AS_W(as_actor(id)+0x3cu)=0;
        }
        FUN_004b1ef0((int32_t)id,1);return;
    }
    if(state==2){
        FUN_004a5da0((int32_t)id,speed);a=as_current();state=AS_B(a+9u);
        if(state==8||state==9||state==10)AS_B(a+6u)=2;
    }else{AS_B(a+0x30u)=1;AS_W(a+0x3cu)=AS_W(a+0x3au);}
}
