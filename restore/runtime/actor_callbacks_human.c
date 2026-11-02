#include "actor_callbacks_human.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
static int32_t add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}
static int32_t mul(int32_t a,int32_t b){return (int32_t)((uint32_t)a*(uint32_t)b);}
static uint32_t actor(uint32_t id){return 0xbcd5f8u+id*388u;}
static uint32_t current(void){return U(0xd056f8);}
static uint8_t water(uint32_t a){uint32_t tile=U(a+32);return (U(0xaf598cu+tile*4u)&4u)&&!B(0xadc36cu+tile);}
static void advance_frame(uint32_t a){B(a+1)=(uint8_t)(B(a+1)+1u);if(B(a+1)>=12)B(a+1)=0;}
typedef struct {uint32_t address;int32_t orientation;} HumanImage;
static HumanImage image(uint8_t immigrant,uint8_t on_water)
{
    HumanImage result;uint32_t a=actor(current()),sprite;int32_t direction=sub((int8_t)B(a+17),I(0xbca2e0));
    if(direction<0)direction=add(direction,8);I(0xe94e00)=direction;
    if(B(a+56)==4) {
        if(on_water) {
            int32_t timer=H(a+54);
            if(timer<33) {
                uint8_t variant=restore_actor_human_boundary(H(a+20),H(a+22));a=actor(current());direction=I(0xe94e00);
                sprite=U(variant?0xf571c4u:0xf56144u)+(uint32_t)(H(a+54)/3)+(uint32_t)(direction/8);
            } else {timer=sub(timer,33);if(timer>=23)timer=22;sprite=U(0xf56e14)+(uint32_t)timer;}
            U(a+4)=sprite;if(!immigrant)U(a+384)=0;
        } else {
            int32_t timer=H(a+54)/2;sprite=U(immigrant?0xf55d64u:0xf55d04u)+B(0x5d048cu+(uint32_t)timer);U(a+4)=sprite;
        }
    } else if(!on_water) {
        U(a+4)=U(immigrant?0xf55d5cu:0xf55cfcu)+(uint32_t)B(a+1)*8u+(uint32_t)direction;
        if(immigrant)U(a+384)=0;
    } else {
        uint8_t variant=restore_actor_human_boundary(H(a+20),H(a+22));a=actor(current());direction=I(0xe94e00);
        U(a+4)=U(variant?0xf571bcu:0xf5613cu)+((uint32_t)B(a+1)/3u)*8u+(uint32_t)direction;U(a+384)=0;
    }
    result.address=a;result.orientation=direction;return result;
}
void restore_actor_00489ca0(void)
{
    uint32_t id=current(),a=actor(id),speed=U(0x7090ecu+(uint32_t)B(a+10)*56u),home;
    int32_t home_id=H(a+92);uint8_t on_water;
    B(a+114)=0;U(a+384)=0;on_water=water(a);home=0x7999d8u+(uint32_t)home_id*264u;
    if(B(home)!=1||(int32_t)H(home+46)!=(int32_t)id||!B(home+5)){B(a+14)=2;return;}
    advance_frame(a);
    switch(B(a+56)) {
    case 5:FUN_004b17d0((int32_t)id);break;
    case 4:FUN_004b1d50((int32_t)id);break;
    case 8:
        B(a+101)=1;B(a+1)=0;W(a+54)=(uint16_t)(W(a+54)-1u);
        if(H(a+54)<=0) {
            uint32_t found=FUN_0047fe60(H(home+6),H(home+8),B(home+3),2);id=current();a=actor(id);
            if(!found)B(a+14)=2;
            else {B(a+56)=9;W(a+36)=W(0xd3b4e4);W(a+38)=W(0xd3b4e0);W(a+68)=0;}
        }
        break;
    case 9:
        B(a+101)=0;FUN_004a5da0((int32_t)id,speed);id=current();a=actor(id);
        switch(B(a+17)) {
        case 9:B(a+56)=8;W(a+54)=1;FUN_004b3250((int32_t)id);break;
        case 10:W(home+46)=0;W(home+32)=0;B(a+14)=2;break;
        case 8:
            B(a+56)=10;W(a+36)=W(home+6);W(a+38)=W(home+8);
            FUN_004b4220((int32_t)id,H(a+74),H(a+76),mul(H(a+36),15),mul(H(a+38),15),0);a=actor(current());W(a+68)=0;break;
        }
        break;
    case 10: {
        int32_t arrived;B(a+12)=1;B(a+101)=1;arrived=FUN_004b3ff0((int32_t)id,1);id=current();a=actor(id);
        if(arrived==1) {
            int32_t capacity=I(0x70ce14u+(uint32_t)(int32_t)H(home+18)*88u),remaining;
            B(a+14)=2;if(B(home+4))capacity=mul(capacity,4);remaining=sub(capacity,H(home+28));if(remaining<0)remaining=0;
            if(remaining<(int32_t)B(a+100))B(a+100)=(uint8_t)remaining;
            if(!H(home+28)&&!I(0x978d70)){FUN_0046cea0(home_id,10);id=current();}
            if(!B(home+135)) {
                uint32_t amount=B(actor(id)+100);W(home+28)=(uint16_t)(W(home+28)+(uint16_t)amount);W(home+30)=(uint16_t)((uint32_t)capacity-W(home+28));
                FUN_004e90f0(I(0xeaac28),(int32_t)amount);id=current();
            }
            W(home+46)=0;
        }
        a=actor(id);B(a+101)=B(a+64)!=0;break;
    }
    default:B(a+56)=8;break;
    }
    image(1,on_water);
}
void restore_actor_0048a260(void)
{
    uint32_t id=current(),a=actor(id),speed=U(0x7090ecu+(uint32_t)B(a+10)*56u);uint8_t on_water;HumanImage result;
    B(a+114)=0;U(a+384)=0;on_water=water(a);advance_frame(a);
    switch(B(a+56)) {
    case 5:FUN_004b17d0((int32_t)id);break;
    case 4:FUN_004b1d50((int32_t)id);break;
    case 8:
        B(a+101)=1;B(a+1)=0;W(a+54)=(uint16_t)(W(a+54)+1u);
        if(H(a+54)>=5) {
            uint32_t found=FUN_0047fe60(H(a+20),H(a+22),1,5);id=current();a=actor(id);
            if(!found)B(a+14)=2;
            B(a+56)=9;W(a+36)=W(0xd3b4e4);W(a+38)=W(0xd3b4e0);
            FUN_004b4220((int32_t)id,H(a+74),H(a+76),mul(H(a+36),15),mul(H(a+38),15),0);a=actor(current());W(a+68)=0;
        }
        break;
    case 9:
        B(a+12)=1;B(a+101)=1;
        {int32_t arrived=FUN_004b3ff0((int32_t)id,1);id=current();a=actor(id);
        if(arrived==1){uint32_t player=U(0xeaac28)*0x49d8u;B(a+56)=10;W(a+36)=W(0xea3fe8u+player);W(a+38)=W(0xea3feau+player);W(a+68)=0;B(a+57)=15;}
        B(a+101)=B(a+64)!=0;}
        break;
    case 10:
        B(a+12)=0;B(a+101)=0;FUN_004a5da0((int32_t)id,speed);a=actor(current());
        if(B(a+17)==9||B(a+17)==10||B(a+17)==8)B(a+14)=2;
        break;
    default:B(a+56)=8;break;
    }
    result=image(0,on_water);a=result.address;
    if(B(a+56)==10) {
        int32_t direction=add(result.orientation,4);if(direction>=8)direction=sub(direction,8);I(0xbcb9a0)=direction;
        B(a+108)=B(0x5dc23cu+(uint32_t)direction*2u);B(a+109)=B(0x5dc23du+(uint32_t)direction*2u);
    }
}
