#include "map_commit_costs.h"
#include "actor_cadence_closure.h"
#include "actor_routing.h"
#include "actor_civic.h"
#include "actor_combat.h"
#include "invasion_model.h"
#include "invasion_commands.h"
#include "byte_fill.h"
#include <stdlib.h>
#include <string.h>

#define C_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define C_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

static void disaster_boundary(uint32_t site,uint32_t target,struct restore_invasion_frame *f)
{
 (void)site;
 if(target==0x5178e0u){
  f->r[0]=restore_rebuild_access_map_result();f->r[4]+=4u;return;
 }
 /* 474380's retained adapter correctly uses its argument high24, not
    incoming scratch EAX. Its early return is the only other 469140 exit.
    All remaining child scratch results are dead or genuine typed queries. */
 restore_invasion_raw(target,f->r,&f->flags);
}

static void cadence_boundary(uint32_t site,uint32_t target,struct restore_routing_frame *f)
{
 uint32_t sp=f->r[4];
 (void)site;
 switch(target){
 case 0x4b27e0u:restore_routing_C_004b27e0(f);return;
 case 0x4b33d0u:restore_routing_C_004b33d0(f);return;
 case 0x4b3530u:restore_routing_C_004b3530(f);return;
 case 0x4b3c60u:restore_routing_C_004b3c60(f);return;
 case 0x4690f0u:restore_routing_C_004690f0(f);return;
 case 0x469140u:{
  struct restore_invasion_frame child;
  memcpy(child.r,f->r,sizeof(child.r));child.flags=f->flags;
  child.boundary=disaster_boundary;
  restore_invasion_C_00469140(&child);
  memcpy(f->r,child.r,sizeof(f->r));f->flags=child.flags;return;
 }
 case 0x4b3f00u:{
  uint32_t actor=C_U(sp+4u);
  /* Native 4B3F04..0C defines EAX=actor*388 and never writes EAX
     again. Reuse its correct memory effects, derive its exact result;
     this is not sampling the incidental return register of a void C call. */
  FUN_004b3f00((int32_t)actor);
  f->r[0]=actor*388u;f->r[4]=sp+4u;return;
 }
 default:
  /* At these edges the retained dispatcher already supplies the genuine
     query values/receivers and native RET cleanup. Other EAX values are
     overwritten before this closure consumes them; see contracts.json. */
  restore_actor_routing_raw(target,f->r,&f->flags);return;
 }
}

void restore_actor_cadence_step_raw(uint32_t registers[8],uint32_t *flags)
{
 struct restore_routing_frame f;
 memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=cadence_boundary;
 restore_routing_C_004b27e0(&f);
 memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

static void cadence_parent_boundary(uint32_t site,uint32_t target,struct restore_civic_frame *f)
{
 (void)site;
 if(target!=0x4b27e0u)abort();
 restore_actor_cadence_step_raw(f->r,&f->flags);
}

void restore_actor_cadence_raw(uint32_t registers[8],uint32_t *flags)
{
 struct restore_civic_frame f;
 memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=cadence_parent_boundary;
 restore_civic_C_004a3b00(&f);
 memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

uint32_t restore_actor_animation_step_eax(int32_t actor,int32_t steps,uint32_t incoming_eax)
{
 uint32_t stack[4096],sp=(uint32_t)(uintptr_t)(stack+4000);
 struct restore_routing_frame f={{0},2,cadence_boundary};
 /* Only entry EAX is live for this result contract: nonpositive steps
    return it unchanged. Positive steps define every other consumed input
    from actor/steps/memory before using it; initial CL is overwritten,
    EBX high24/entry flags never affect the consumed result or effects. */
 stack[4000]=0;stack[4001]=(uint32_t)actor;stack[4002]=(uint32_t)steps;
 f.r[0]=incoming_eax;f.r[4]=sp;
 restore_routing_C_004b27e0(&f);
 if(f.r[4]!=sp+4u)abort();
 return f.r[0];
}
