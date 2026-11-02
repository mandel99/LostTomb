#include "actor_callbacks_human.h"
#include "actor_walking.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
extern uint8_t FUN_0051cc00(int32_t,int32_t,int32_t,int32_t);
void FUN_004b4220(int32_t actor,int32_t from_x,int32_t from_y,int32_t target_x,int32_t target_y,int32_t mode)
{
    uint32_t a=(uint32_t)actor*388u,dx,dy,error,direction;int32_t sx,sy;
    W(0xbcd646u+a)=(uint16_t)target_x;W(0xbcd648u+a)=(uint16_t)target_y;
    dx=from_x>target_x?(uint32_t)from_x-(uint32_t)target_x:(uint32_t)target_x-(uint32_t)from_x;W(0xbcd64au+a)=(uint16_t)dx;
    dy=from_y>target_y?(uint32_t)from_y-(uint32_t)target_y:(uint32_t)target_y-(uint32_t)from_y;W(0xbcd64cu+a)=(uint16_t)dy;
    dy=W(0xbcd64cu+a);dx=W(0xbcd64au+a);
    if((int16_t)dx<(int16_t)dy)error=dx*2u-dy;
    else if((int16_t)dx>(int16_t)dy)error=dy*2u-dx;
    else error=0;
    W(0xbcd64eu+a)=(uint16_t)error;
    if(mode){direction=FUN_0051cc00(from_x,from_y,target_x,target_y);B(0xbcd609u+a)=(uint8_t)direction;}
    else{
        direction=(uint8_t)FUN_0051cba0(from_x,from_y,target_x,target_y);B(0xbcd609u+a)=(uint8_t)direction;
        sx=(int16_t)W(0xbcd64au+a);sy=(int16_t)W(0xbcd64cu+a);
        if(sy>sx*2){
            if(direction==1u||direction==7u)B(0xbcd609u+a)=0;
            else if(direction==3u||direction==5u)B(0xbcd609u+a)=4;
        }
        sy=(int16_t)W(0xbcd64cu+a);sx=(int16_t)W(0xbcd64au+a);
        if(sx>sy*2){
            direction=B(0xbcd609u+a);
            if(direction==1u||direction==3u)B(0xbcd609u+a)=2;
            else if(direction==5u||direction==7u)B(0xbcd609u+a)=6;
        }
    }
    sy=(int16_t)W(0xbcd64cu+a);sx=(int16_t)W(0xbcd64au+a);B(0xbcd650u+a)=sy>sx?2u:1u;
}
