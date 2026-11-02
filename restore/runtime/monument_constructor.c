#include "monument_constructor.h"
#include "preview_footprint_seeds.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
static const uint32_t offsets[8]={0x4c,0x4c,0x4c,0x4c,0x1e5c,0x1cbc,0x4ee0,0x5c88};
static const uint32_t counts[8]={25,16,9,2,167,158,437,513};

/* Whole native4E50F0, a code label missed by the function manager. */
void *restore_monument_zero_record(void *object)
{
    volatile uint32_t *d=object;uint32_t i;
    for(i=0;i<5u;++i)d[i]=0;
    for(i=0;i<4u;++i){d[i+5u]=0;d[i+9u]=0;}
    return object;
}
void restore_monument_class_clear(void *object,uint32_t class_index)
{
    uint32_t i;
    for(i=0;i<counts[class_index];++i)
        (void)restore_monument_zero_record((void *)(uintptr_t)((uint32_t)(uintptr_t)object+offsets[class_index]+i*52u));
}

/* Whole4E5140 copy order. Forward overlap is deliberately not memmove. */
void *restore_monument_copy_record(void *destination,const void *source)
{
    volatile uint32_t *d=destination;const volatile uint32_t *s=source;uint32_t i;
    for(i=0;i<5u;++i)d[i]=s[i];
    for(i=0;i<4u;++i){d[i+5u]=s[i+5u];d[i+9u]=s[i+9u];}
    return destination;
}

/* Whole4E5210, optionally followed by545F80. Only the destructor51F780
   is RET-only;4E50F0 first clears all thirteen fields of the array element. */
void restore_monument_class_base(void *object,RestoreMonumentRecord *temporary,int large)
{
    uint32_t self=(uint32_t)(uintptr_t)object;
    (void)restore_monument_zero_record((void *)(uintptr_t)(self+12u));
    U(self)=0x56f698u;
    temporary->fields[0]=0;temporary->fields[1]=0;
    temporary->fields[2]=1;temporary->fields[3]=7;
    (void)restore_monument_copy_record((void *)(uintptr_t)(self+12u),temporary);
    U(self+0x40u)=0x5e4578u;U(self+0x44u)=0x5e456cu;U(self+0x48u)=0x5e4560u;
    U(self+4u)=1;U(self+8u)=0;
    if(large)U(self)=0x5701a0u;
}

void restore_monument_class_seed(void *object,uint32_t class_index,RestoreMonumentRecord *temporary)
{
    uint32_t i;int32_t seed[4];
    for(i=0;restore_preview_footprint_seed(class_index,i,seed);++i){
        uint32_t j;
        for(j=0;j<4u;++j)temporary->fields[j]=(uint32_t)seed[j];
        (void)restore_monument_copy_record((void *)(uintptr_t)((uint32_t)(uintptr_t)object+offsets[class_index]+i*52u),temporary);
    }
}

void restore_monument_constructors_initialize(void)
{
    /* Original static initializer entry order413C80 through413E40. */
    (void)FUN_0040d280((void *)0x6fd5d8u);(void)FUN_00416c20((void *)0x6fd368u);
    (void)FUN_005430e0((void *)0x6f9320u);(void)FUN_005567d0((void *)0x6f5648u);
    (void)FUN_005480e0((void *)0x6e9188u);(void)FUN_0054fef0((void *)0x6de9e0u);
    (void)FUN_00401030((void *)0x6de650u);(void)FUN_004e6400((void *)0x6de548u);
}
