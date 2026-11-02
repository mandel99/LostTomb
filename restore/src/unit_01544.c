#include "flood_simulation.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
void FUN_004bd380(uintptr_t self)
{
    static const int32_t weights[11]={2,3,5,10,15,30,15,10,5,3,2};
    static const int32_t changes[11]={100,80,60,40,20,0,-20,-40,-60,-80,-100};
    uint32_t a=U(0xd3b384),b=U(0xd3b380),i,cumulative=0;
    int32_t change=9999,value;uint32_t index=U(0xd3b1c4);
    U(0xd3b1c8+index*4u)=U(0xd3b374);index++;
    U(0xd3b1c4)=(int32_t)index<100?index:0;
    for(i=0;i<31;++i){
        uint32_t x=((a>>4)^a)&1u,y=((b>>4)^b)&1u;
        a=(a>>1)|(x<<30);b=(b>>1)|(y<<30);
    }
    U(0xd3b384)=a;U(0xd3b380)=b;
    U(0xd3b364)=b&0x7fff;U(0xd3b368)=b&0x7f;U(0xd3b36c)=b&7;
    U(0xd3b370)=a&0x7fff;U(0xd3b374)=a&0x7f;U(0xd3b378)=a&7;
    for(i=0;i<11;++i){
        cumulative+=(uint32_t)weights[i];
        if ((a&0x7fff)%99u+1u<cumulative){change=changes[i];break;}
    }
    value=(int32_t)(U(self+0x1c)+(uint32_t)change);
    U(self+0x1c)=value>=100?100:value<=0?0:(uint32_t)value;
}
