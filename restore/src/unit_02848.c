#include "byte_string_compare.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Native5631B0 compares unsigned bytes and returns exactly-1/0/+1. Retain
   aligned left-hand word loads, early NUL exits and native read widths. */
int32_t restore_byte_string_compare(const char *left,const char *right)
{
    uint32_t l=(uint32_t)(uintptr_t)left,r=(uint32_t)(uintptr_t)right,value,i;
    uint8_t a,b;
    if(l&3u){
        if(l&1u){
            a=B(l);++l;b=B(r);
            if(a!=b)return a<b?-1:1;
            ++r;if(!a)return 0;
            if(!(l&2u))goto aligned;
        }
        value=W(l);l+=2u;
        a=(uint8_t)value;b=B(r);
        if(a!=b)return a<b?-1:1;
        if(!a)return 0;
        a=(uint8_t)(value>>8);b=B(r+1u);
        if(a!=b)return a<b?-1:1;
        if(!a)return 0;
        r+=2u;
    }
aligned:
    for(;;){
        value=U(l);
        for(i=0;i<4u;++i){
            a=(uint8_t)(value>>(i*8u));b=B(r+i);
            if(a!=b)return a<b?-1:1;
            if(!a)return 0;
        }
        l+=4u;r+=4u;
    }
}
int FUN_005631b0(const char *left,const char *right)
{return restore_byte_string_compare(left,right);}
