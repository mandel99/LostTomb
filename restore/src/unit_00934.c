#include "city_capacity.h"
#include "city_prepare.h"
#include "city_updates.h"
#include "campaign_score_numeric.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Native4683FF..46840B: FILD32/FIDIV32 at the inherited x87 PC/RC,
   followed by the original signed64 truncation and consumed low32 bits.
   Raw80 introduces no precision spill. Zero divisors reach native x87. */
static uint32_t average_coverage(int32_t sum,int32_t active)
{
    uint8_t raw80[10];
    __asm {
        fild sum
        fidiv active
        lea eax,raw80
        fstp tbyte ptr[eax]
    }
    return (uint32_t)restore_campaign_score_trunc80(raw80);
}

void restore_city_rebuild_service_coverage(void)
{
    static const uint32_t input[5]={0x7057b8u,0x7057b4u,0x7057c0u,0x7057bcu,0x705ac4u};
    static const uint32_t weight[5]={400u,700u,1200u,5000u,7500u};
    uint32_t i,denominator,numerator,value,first,mode,at,a,b,c,active=0,sum=0,owner,off;
    for(i=0;i<4u;++i){
        denominator=U(0xea1894u+U(0xeaac28u)*0x49d8u);
        numerator=U(input[i])*weight[i];
        U(0x78d5f0u+i*4u)=(uint32_t)FUN_004cde60(I(numerator),I(denominator));
    }
    denominator=U(0xea1894u+U(0xeaac28u)*0x49d8u);
    numerator=U(input[4])*weight[4];
    value=(uint32_t)FUN_004cde60(I(numerator),I(denominator));
    first=U(0x78d5f0u);U(0x78d600u)=value;
    if(I(first)>100)U(0x78d5f0u)=100;
    for(i=1;i<4u;++i)if(I(U(0x78d5f0u+i*4u))>100)U(0x78d5f0u+i*4u)=100;
    if(I(value)>100)U(0x78d600u)=100;
    for(i=0;i<5u;++i){
        at=0x78d620u+i*4u;mode=W(0x7848e2u+i*2u);
        if(mode==2u||mode==1u){
            denominator=U(0xea1894u+U(0xeaac28u)*0x49d8u);
            a=FUN_004c06e0(i,1);numerator=U(0x70573cu+a*4u)*8000u;
            b=FUN_004c06e0(i,0);numerator+=U(0x70573cu+b*4u)*(mode==2u?375u:750u);
            c=FUN_004c06e0(i,2);numerator+=U(0x70573cu+c*4u)*(mode==2u?150u:300u);
            U(at)=(uint32_t)FUN_004cde60(I(numerator),I(denominator));++active;
        }else U(at)=0;
        if(I(U(at))>100)U(at)=100;
        sum+=U(at);
    }
    value=average_coverage(I(sum),I(active));owner=U(0xeaac28u);U(0x78d60cu)=value;
    value=(uint32_t)FUN_004e94a0(I(owner));owner=U(0xeaac28u);off=owner*0x49d8u;
    U(0xea189cu+off)=value;
    value=(uint32_t)FUN_004e94d0(I(owner));U(0xea18a0u+off)=value;
    denominator=U(0xea189cu+off);numerator=U(0x705808u)*300u;
    value=(uint32_t)FUN_004cde60(I(numerator),I(denominator));owner=U(0xeaac28u);U(0x78d604u)=value;
    denominator=U(0xea1894u+owner*0x49d8u);numerator=U(0x705810u)*800u;
    value=(uint32_t)FUN_004cde60(I(numerator),I(denominator));first=U(0x78d604u);U(0x78d608u)=value;
    if(I(first)>100)U(0x78d604u)=100;if(I(value)>100)U(0x78d608u)=100;
    FUN_004e7e60((uint32_t *)(uintptr_t)0x78d614u,(uint32_t *)(uintptr_t)0x78d61cu,
                 (uint32_t *)(uintptr_t)0x78d618u,(uint32_t *)(uintptr_t)0x78d610u);
}
