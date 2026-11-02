#include "actor_constable.h"
#include "actor_civic.h"
#include "actor_laborer.h"
#include "actor_routing.h"
#include "actor_combat.h"
#include "actor_callbacks_human.h"
#include "actor_walking.h"
#include "actor_order_path.h"
#include <stdlib.h>
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

void restore_actor_constable_dispatch(uint32_t site,uint32_t target,struct restore_constable_frame *f)
{
    uint32_t sp=f->r[4];(void)site;
#define A(n) U(sp+4u*(n))
#define RAW(a) case 0x##a##u:restore_constable_C_##a(f);return
    switch(target){
    RAW(004abe40);RAW(0048e6f0);RAW(0048e820);RAW(00518b30);
    case 0x4b4220u:case 0x4b46b0u:restore_actor_civic_raw(target,f->r,&f->flags);return;
    case 0x4b3ff0u:case 0x4b1df0u:case 0x4b1ef0u:case 0x4b3250u:
        restore_actor_laborer_raw(target,f->r,&f->flags);return;
    case 0x4a5da0u:restore_actor_routing_raw(target,f->r,&f->flags);return;
    case 0x4b17d0u:FUN_004b17d0((int32_t)A(1));break;
    case 0x4b1d50u:FUN_004b1d50((int32_t)A(1));break;
    case 0x47fe60u:f->r[0]=FUN_0047fe60((int32_t)A(1),(int32_t)A(2),A(3),(int32_t)A(4));break;
    case 0x51aff0u:f->r[0]=FUN_0051aff0((int32_t)A(1));break;
    case 0x40f8f0u:FUN_0040f8f0((uint32_t *)(uintptr_t)A(1),A(2),(uint8_t)A(3));break;
    case 0x53b4a0u:FUN_0053b4a0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    default:abort();
    }
    /* Genuine RET0 typed queries expose full EAX. Other continuations reload
       consumed volatile registers; native movement/routing share the frame. */
    f->r[4]=sp+4u;
#undef RAW
#undef A
}
void restore_actor_constable_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_constable_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=restore_actor_constable_dispatch;
    restore_actor_constable_dispatch(0,target,&f);
    memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}
static uint32_t invoke(uint32_t entry,const uint32_t *args,uint32_t count)
{
    uint32_t stack[4096],sp=(uint32_t)(uintptr_t)(stack+4000),i;
    struct restore_constable_frame f={{0},2,restore_actor_constable_dispatch};
    stack[4000]=0;f.r[4]=sp;
    for(i=0;i<count;++i)stack[4001+i]=args[i];
    restore_actor_constable_dispatch(0,entry,&f);
    if(f.r[4]!=sp+4u)abort();
    return f.r[0];
}
void FUN_004abe40(void){(void)invoke(0x4abe40u,0,0);}
uint32_t FUN_0048e6f0(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};return invoke(0x48e6f0u,a,1);}
int32_t FUN_0048e820(int32_t x,int32_t y,int32_t radius)
{const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)radius};return (int32_t)invoke(0x48e820u,a,3);}
int32_t FUN_00518b30(int32_t x,int32_t y,const int32_t *tiles,int32_t count)
{
    const uint32_t a[]={(uint32_t)x,(uint32_t)y,(uint32_t)(uintptr_t)tiles,(uint32_t)count};
    return (int32_t)invoke(0x518b30u,a,4);
}
