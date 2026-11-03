#include "flood_settings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(a))
extern int32_t FUN_004c5120(int32_t building);

int32_t restore_flood_quality(uintptr_t self)
{
    uint32_t player=U(0xeaac28u)*0x49d8u;
    int32_t quality=(int32_t)U(self+20u);
    if(W(0xea5ce8u+player)&&!FUN_004c5120((int32_t)U(0xea5f9cu+player)))
        quality=(int32_t)((uint32_t)quality+10u);
    return quality>100?100:quality;
}
void restore_flood_settings_roll_quality(uintptr_t self)
{
    static const uint32_t weights[11]={2,3,5,10,15,30,15,10,5,3,2};
    static const int32_t changes[11]={100,80,60,40,20,0,-20,-40,-60,-80,-100};
    uint32_t index=U(0xd3b1c4u),previous=U(0xd3b374u),a,b,i,sum=0,roll,value;
    int32_t change=9999;
    U(0xd3b1c8u+index*4u)=previous;++index;U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
    a=U(0xd3b384u);b=U(0xd3b380u);
    for(i=0;i<31;++i){
        uint32_t x=((a>>4)^a)&1u,y=((b>>4)^b)&1u;
        a=(a>>1)|(x<<30);b=(b>>1)|(y<<30);
    }
    U(0xd3b380u)=b;U(0xd3b374u)=a&0x7fu;U(0xd3b364u)=b&0x7fffu;
    U(0xd3b370u)=a&0x7fffu;U(0xd3b36cu)=b&7u;U(0xd3b368u)=b&0x7fu;
    roll=(a&0x7fffu)%99u+1u;
    U(0xd3b384u)=a;U(0xd3b378u)=a&7u;
    for(i=0;i<11;++i){sum+=weights[i];if(roll<sum){change=changes[i];break;}}
    value=U(self+28u)+(uint32_t)change;
    if((int32_t)value>=100)value=100;
    U(self+28u)=value;
    if((int32_t)value<=0)value=0;
    U(self+28u)=value;
}
void restore_flood_settings_prepare(uintptr_t self)
{
    uint32_t start=U(self+4u),duration=U(self),quality=U(self+8u);
    U(self+16u)=start;U(self+12u)=duration;U(self+28u)=quality;
    U(self+32u)=(uint32_t)restore_flood_quality(self);
    restore_flood_settings_roll_quality(self);
}
