#include "actor_callback_496140.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
static uint32_t actor(uint32_t id){return 0xbcd5f8u+id*388u;}
static void image(void)
{
    uint32_t a=actor(U(0xd056f8)),sprite;int32_t direction=(int32_t)((uint32_t)(int32_t)(int8_t)B(a+17)-U(0xbca2e0));
    if(direction<0)direction=(int32_t)((uint32_t)direction+8u);I(0xe94e00)=direction;
    if(B(a+56)==4)sprite=U(0xf55d74)+B(0x5d048cu+(uint32_t)(H(a+54)/2));
    else sprite=U(0xf55d6c)+(uint32_t)B(a+1)*8u+(uint32_t)direction;
    U(a+4)=sprite;
}
static void return_goal(uint32_t id,uint32_t a)
{B(a+56)=9;W(a+36)=W(a+44);W(a+38)=W(a+46);FUN_004b3250((int32_t)id);}
void restore_actor_00496140(void)
{
    uint32_t id=U(0xd056f8),a=actor(id),speed=U(0x7090ecu+(uint32_t)B(a+10)*56u),b;
    int32_t building=H(a+90);B(a+114)=1;B(a+12)=0;W(a+66)=800;b=0x7999d8u+(uint32_t)building*264u;
    if(B(b)!=1)B(a+14)=2;
    if((int32_t)H(b+44)!=(int32_t)id&&(int32_t)H(b+182)!=(int32_t)id)B(a+14)=2;
    B(a+1)=(uint8_t)(B(a+1)+1u);if(B(a+1)>=12)B(a+1)=0;
    switch(B(a+56)) {
    case 4:FUN_004b1d50((int32_t)id);break;
    case 5:FUN_004b17d0((int32_t)id);break;
    case 6:U(a+4)=0;break;
    case 7:
        B(a+1)=0;W(a+54)=(uint16_t)(W(a+54)+1u);
        if(H(a+54)>100){W(a+54)=0;if(!(uint8_t)FUN_004a35e0((int32_t)id))B(actor(U(0xd056f8))+56)=9;}
        break;
    case 8:
        FUN_004a5da0((int32_t)id,speed);id=U(0xd056f8);a=actor(id);
        if(B(a+17)==8)FUN_00496060((int32_t)id,(uint32_t)building,(uint32_t)(int32_t)H(a+94));
        else if((int8_t)B(a+17)>8&&(int8_t)B(a+17)<=10)return_goal(id,a);
        break;
    case 9:
        FUN_004a5da0((int32_t)id,speed);id=U(0xd056f8);a=actor(id);
        if(B(a+17)==8||B(a+17)==10)B(a+14)=2;else if(B(a+17)==9)FUN_004b3250((int32_t)id);
        break;
    default:return_goal(id,a);break;
    }
    image();
}
