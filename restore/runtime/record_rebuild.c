#include "record_place.h"
#include "map_validation.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f8f0(void *,uint32_t,uint8_t);
void FUN_00444720(void)
{
    uint32_t i,p=0x77de6du,r=0x77f748u,j,a,v;uint8_t x,y,z;uint16_t w;
    FUN_0040f8f0((void *)(uintptr_t)0x77de00u,0x1942,0);
    for(i=0;r<0x7843d8u;++i,r+=98u){
        if(!B(r+1u)||B(r)!=1u)continue;
        if(p>=0x77f745u)return;
        x=B(r+24u);y=B(r+25u);B(p-3u)=1;B(p-1u)=x;z=B(r+26u);B(p)=y;
        if(z>=20u)B(r+26u)=19;
        x=B(r+26u);y=B(r+27u);w=W(r+28u);B(p+1u)=x;B(p+2u)=y;W(p+75u)=w;
        x=B(p+1u);B(p+85u)=(uint8_t)FUN_00444950((int32_t)x);
        for(j=1;j<36u;++j){
            B(p+j+39u)=0;B(p+j+3u)=0;x=B(p-1u);
            if(x==1u||x==3u||x==5u||B(p-1u)==0u){
                if(FUN_00443000(i,j))B(p+j+39u)=1;
                if(FUN_00443040(i,j))B(p+j+3u)=1;
            }
            v=FUN_00444a00(i,j);
            a=(j+B(p+1u)*36u)*4u;
            U(0x77646cu+a)=v==1u?15u:v==2u?25u:v==3u?40u:0u;
            U(0x77592cu+a)=0;
        }
        W(p+77u)=10;W(p+79u)=4;W(p+81u)=0;
        W(p+87u)=0;W(p+89u)=0;W(p+91u)=0;W(p+83u)=(uint16_t)i;p+=106u;
    }
}
