#include "mission_compat.h"
#include <stddef.h>

extern void *FUN_00561a1d(uint32_t bytes);
extern void FUN_00561a2b(void *allocation);
#define DYN_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define DYN_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define ADDRESS(p) ((uint32_t)(uintptr_t)(p))

RestoreDynamicHistory *FUN_004f6670(RestoreDynamicHistory *self)
{
    uint32_t q=ADDRESS(self);
    DYN_U(q+4u)=1;
    DYN_U(q+8u)=0;
    DYN_U(q+12u)=0;
    DYN_U(q+20u)=0;
    DYN_U(q+16u)=0;
    DYN_B(q)=0;
    return self;
}

void FUN_004f6760(RestoreDynamicHistory *self)
{
    uint32_t q=ADDRESS(self),buffer=DYN_U(q+20u);
    if(buffer) {
        FUN_00561a2b((void *)(uintptr_t)buffer);
        DYN_U(q+20u)=0;
    }
    DYN_U(q+16u)=0;
    DYN_U(q+8u)=0;
    DYN_U(q+12u)=0;
    DYN_U(q+4u)=1;
}

void FUN_004f66f0(RestoreDynamicHistory *self){FUN_004f6760(self);}

uint8_t FUN_004f6700(RestoreDynamicHistory *self,int32_t capacity)
{
    uint32_t q=ADDRESS(self),buffer,bytes,words,i;
    if(capacity<=(int32_t)DYN_U(q+16u))return 1;
    FUN_004f6760(self);
    bytes=(uint32_t)capacity*4u;
    DYN_U(q+16u)=(uint32_t)capacity;
    buffer=ADDRESS(FUN_00561a1d(bytes));
    DYN_U(q+20u)=buffer;
    words=bytes>>2;
    for(i=0;i<words;++i)DYN_U(buffer+i*4u)=0;
    DYN_U(q+4u)=1;
    DYN_U(q+8u)=0;
    DYN_U(q+12u)=0;
    return 1;
}

RestoreDynamicHistory *FUN_004f6690(RestoreDynamicHistory *self,int32_t capacity)
{
    uint32_t q=ADDRESS(self);
    DYN_U(q+8u)=0;
    DYN_U(q+12u)=0;
    DYN_U(q+20u)=0;
    DYN_U(q+16u)=0;
    DYN_B(q)=0;
    DYN_U(q+4u)=1;
    (void)FUN_004f6700(self,capacity);
    return self;
}

void FUN_004f6790(RestoreDynamicHistory *self,const RestoreDynamicHistory *source)
{
    uint32_t q=ADDRESS(self),s=ADDRESS(source),input,output,words,i;
    (void)FUN_004f6700(self,(int32_t)DYN_U(s+16u));
    DYN_U(q+12u)=DYN_U(s+12u);
    DYN_U(q+4u)=DYN_U(s+4u);
    DYN_U(q+8u)=DYN_U(s+8u);
    DYN_B(q)=DYN_B(s);
    words=(DYN_U(q+16u)*4u)>>2;
    input=DYN_U(s+20u);
    output=DYN_U(q+20u);
    /* Original 4F67B7 copies the destination capacity, with forward DWORD
       order. Its source extent is not bounded by the source capacity. */
    for(i=0;i<words;++i)DYN_U(output+i*4u)=DYN_U(input+i*4u);
}

RestoreDynamicHistory *FUN_004f66c0(RestoreDynamicHistory *self,const RestoreDynamicHistory *source)
{
    uint32_t q=ADDRESS(self);
    DYN_U(q+8u)=0;
    DYN_U(q+12u)=0;
    DYN_U(q+20u)=0;
    DYN_U(q+16u)=0;
    DYN_B(q)=0;
    DYN_U(q+4u)=1;
    FUN_004f6790(self,source);
    return self;
}

/* Literal native growth entry. Gameplay's existing repaired growth adapter
   remains distinct: the native temporary copy has a smaller readable extent. */
uint32_t FUN_004f6a50(RestoreDynamicHistory *self,int32_t capacity)
{
    if(capacity>(int32_t)DYN_U(ADDRESS(self)+16u)) {
        RestoreDynamicHistory backup;
        FUN_004f66c0(&backup,self);
        (void)FUN_004f6700(self,capacity);
        FUN_004f6790(self,&backup);
        FUN_004f66f0(&backup);
    }
    return 1;
}

void FUN_00451a70(void)
{
    uint32_t i;
    for(i=0;i<5u;++i)
        FUN_004f6690((RestoreDynamicHistory *)(uintptr_t)(0x9783c0u+i*28u),20);
    for(i=0;i<35u;++i)DYN_U(0x97844cu+i*4u)=0;
    /* Native vector constructor 561441 invokes the no-throw default
       constructor five times on this exact mapped global array. */
    for(i=0;i<5u;++i)
        FUN_004f6670((RestoreDynamicHistory *)(uintptr_t)(0x97844cu+i*28u));
}
