#include "financial_updates.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern int32_t FUN_004cde40(int32_t,int32_t),FUN_004cde60(int32_t,int32_t);
static uint32_t offset(void){return U(0xeaac28u)*0x49d8u;}
static uint32_t scale(void){return U(0x5d1974u+U(0xe39080u)*4u);}

/* Whole4599D0/212 instructions. Classification and metric precede the
   optional110-percent query; house kind and coverage are reloaded after it.
   The scan's owner changes only at that call, and its high-water limit is
   live. All counters, history products and money arithmetic wrap at32 bits. */
void FUN_004599d0(void)
{
    static const uint32_t reset_fields[8]={0xea4490u,0xea4494u,0xea4498u,0xea449cu,
        0xea44c8u,0xea44c4u,0xea44d0u,0xea44ccu};
    static const uint32_t values[4]={0xea44ccu,0xea44c4u,0xea44d0u,0xea44c8u};
    static const uint32_t ledgers[4]={0xea44b0u,0xea44acu,0xea44b8u,0xea44b4u};
    uint32_t owner=U(0xeaac28u),off=owner*0x49d8u,i,p,high,metric,count,weighted;
    uint32_t value,total=0,first,second,result,address;
    int32_t kind;
    for(i=0;i<8u;++i)U(reset_fields[i]+off)=0;
    for(i=0;i<20u;++i)U(0xea3efcu+off+i*4u)=0;
    if(I(U(0x78d790u))>=1){
        i=1;p=0x799af2u;
        do{
            if(B(p-18u)==1u&&B(p-13u)){
                kind=H(p);high=kind>=14;metric=U(0x70ce18u+(uint32_t)kind*88u);
                if(B(p+118u)){metric=(uint32_t)FUN_004cde40(I(metric),110);owner=U(0xeaac28u);}
                off=owner*0x49d8u;kind=H(p);count=(uint32_t)(int32_t)H(p+10u);
                U(0xea3efcu+off+(uint32_t)kind*4u)+=count;
                weighted=count*metric;
                if(B(p+60u)){
                    U((high?0xea4494u:0xea4490u)+off)+=count;
                    value=scale()*U(0xea1878u+off)*weighted;U(p+122u)+=value;
                    U((high?0xea44ccu:0xea44c4u)+off)+=weighted;
                }else{
                    U((high?0xea449cu:0xea4498u)+off)+=count;
                    U((high?0xea44d0u:0xea44c8u)+off)+=weighted;
                }
            }
            ++i;p+=264u;
        }while(I(i)<=I(U(0x78d790u)));
    }
    off=owner*0x49d8u;
    for(i=0;i<4u;++i){
        value=(uint32_t)FUN_004cde40(I(U(values[i]+off)),I(U(0xea1878u+off)));
        value=(uint32_t)FUN_004cde40(I(value),I(scale()));off=offset();
        U(ledgers[i]+off)+=value;if(i<2u)total+=value;
    }
    U(0xea187cu+off)+=total;
    first=U(0xea4490u+off);second=U(0xea4498u+off)+first;
    result=(uint32_t)FUN_004cde60(I(first),I(second));U(0xea44a0u+off)=result;
    /* Native takes the second numerator from the prior cached city before
       reloading the owner for its denominator and result destination. */
    first=U(0xea4494u+off);off=offset();second=U(0xea449cu+off)+first;
    result=(uint32_t)FUN_004cde60(I(first),I(second));U(0xea44a4u+off)=result;
    second=U(0xea449cu+off)+U(0xea4498u+off);first=U(0xea4490u+off);
    value=U(0xea4494u+off);second+=first;first+=value;second+=value;
    result=(uint32_t)FUN_004cde60(I(first),I(second));address=0xea44a8u+offset();U(address)=result;
}

/* Whole459D20/79 instructions. Projection uses the entry scan limit, no
   bonus query, and the full DWORD month. Actual percentage calls determine
   where each subsequent city read belongs. */
void FUN_00459d20(void)
{
    uint32_t off=offset(),count=U(0x78d790u),p=0x799af2u,first,second,value,other;
    int32_t kind;
    U(0xea44c4u+off)=0;U(0xea44ccu+off)=0;
    if(I(count)>=1)do{
        if(B(p-18u)==1u&&B(p-13u)){
            kind=H(p);value=U(0x70ce18u+(uint32_t)kind*88u);
            if(B(p+60u)){
                value*=(uint32_t)(int32_t)H(p+10u);
                U((kind>=14?0xea44ccu:0xea44c4u)+off)+=value;
            }
        }
        p+=264u;
    }while(--count);
    first=(uint32_t)FUN_004cde40(I(U(0xea44ccu+off)),I(U(0xea1878u+off)));
    first=(uint32_t)FUN_004cde40(I(first),I(scale()));off=offset();
    second=(uint32_t)FUN_004cde40(I(U(0xea44c4u+off)),I(U(0xea1878u+off)));
    second=(uint32_t)FUN_004cde40(I(second),I(scale()));off=offset();
    other=U(0xea44acu+off)+U(0xea44b0u+off);U(0xea44bcu+off)=other;
    U(0xea5e5cu+off)=(first+second)*(12u-U(0x78d7c0u))+other;
}
