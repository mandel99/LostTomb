#include "random_building_patrol.h"
#include "menu_event_actions.h"
#include "menu_resource_render.h"
#include "editor_event_list_actions.h"
#include "editor_event_detail_actions.h"
#include "sidebar_undo.h"
#include "invasion_commands.h"
#include "diplomacy_input_actions.h"
#include "menu_advisor_actions.h"
#include "menu_economy_actions.h"
#include "town_menu.h"
#include "order_input.h"
#include "actor_order_los.h"
#include "map_record_refresh.h"
#include "map_validation.h"
#include "record_place.h"
#include "command_migration.h"
#include "building_footprint.h"
#include "actor_create.h"
#include "actor_walking.h"
#include "actor_combat.h"
#include "map_commit_links.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "event_publisher.h"
#include "message_panel_input.h"
#include "menu_full_input_numeric.h"
#include "legacy_format.h"
#include "sound_streams.h"
#include "mission_unlock.h"
#include "resource_availability.h"
#include "actor_mission_init.h"
#include "event_packet.h"
#include "event_parameters.h"
#include "event_schedule.h"
#include <stdlib.h>
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_0047d350(void);
extern void FUN_004e96c0(int32_t amount);

static void dispatch(uint32_t site,uint32_t target,struct restore_event_actions_frame *f)
{
    uint32_t sp=f->r[4],cleanup=0;(void)site;
#define A(n) U(sp+4u*(n))
#define RAW(address) case 0x##address##u:restore_event_C_##address(f);return
    switch(target){
    RAW(0044ba90);
    RAW(00446260);
    RAW(004496d0);
    RAW(0044c0d0);
    RAW(0044c430);
    RAW(0044c660);
    RAW(0044c7e0);
    RAW(0044c8a0);
    RAW(0044c960);
    RAW(0044c9a0);
    RAW(0044c9e0);
    RAW(0044cdc0);
    RAW(0044d780);
    RAW(0044d870);
    RAW(0044d970);
    RAW(0044da30);
    RAW(0044db20);
    RAW(0044dbd0);
    RAW(0044dca0);
    RAW(0044dd60);
    RAW(0044de20);
    RAW(0044dee0);
    RAW(0044df90);
    RAW(0044e000);
    RAW(0044e070);
    RAW(0044e0e0);
    RAW(0044e150);
    RAW(0044e1c0);
    RAW(0044e230);
    RAW(0044e2a0);
    RAW(0044e5d0);
    RAW(0044e6f0);
    RAW(0044e890);
    RAW(0044edd0);
    RAW(0044ee30);
    RAW(0044eea0);
    RAW(0044eef0);
    RAW(0044ef60);
    RAW(0044ef90);
    RAW(0044f120);
    RAW(0044f170);
    RAW(0044f360);
    RAW(0044f3d0);
    RAW(0044f440);
    RAW(0044f470);
    RAW(0044f660);
    RAW(0044f8c0);
    RAW(0044fb20);
    RAW(0044fc00);
    RAW(0044fce0);
    RAW(0044fd30);
    RAW(0044fec0);
    RAW(00451540);
    RAW(004583f0);
    RAW(0045aac0);
    RAW(00469f80);
    RAW(004b1df0);
    RAW(004b8330);
    RAW(004b8360);
    RAW(004b8690);
    RAW(004b8890);
    RAW(004bd5e0);
    RAW(004bed00);
    RAW(004bed40);
    RAW(004e6eb0);
    RAW(004e6ed0);
    RAW(004e70b0);
    RAW(004e7200);
    RAW(004e7300);
    RAW(004e7410);
    RAW(004e75a0);
    RAW(004e7ac0);
    RAW(004e7c90);
    RAW(004e85d0);
    RAW(004e8e80);
    RAW(004eada0);
    RAW(004eb800);
    RAW(00562348);
    case 0x444980u:case 0x4449d0u:case 0x449390u:
    case 0x44c110u:case 0x44d700u:case 0x44d750u:
        restore_resource_render_raw(target,f->r,&f->flags);return;
    case 0x45afe0u:case 0x56f0f8u:
        /* The existing debug-output adapter captures its real OS EAX and
           uses RET4. No fabricated return for that void Win32 boundary. */
        restore_sidebar_raw(target,f->r,&f->flags);return;
    case 0x449550u:
        restore_event_detail_raw(target,f->r,&f->flags);return;
    case 0x446a30u:
        restore_invasion_raw(target,f->r,&f->flags);return;
    case 0x4b8460u:
        restore_diplomacy_input_raw(target,f->r,&f->flags);return;
    case 0x4c38f0u:
        restore_advisor_raw(target,f->r,&f->flags);return;
    /* Reset's callers replace flags/EDX before consuming them. Its PUSH ECX
       allocates a loop-counter local; the final POP ECX publishes zero. */
    case 0x4487f0u:f->r[0]=restore_event_parameter_choose(f->r[1],f->r[0]);break;
    case 0x448a90u:f->r[0]=restore_event_record_reset(f->r[1]);f->r[1]=0;break;
    /* Only state is consumed after these native void methods. */
    case 0x44bc20u:restore_event_rearm_completed(f->r[1]);break;
    case 0x44c490u:restore_event_schedule_apply(f->r[1],(int32_t)A(1),(int16_t)A(2),f);cleanup=8;break;
    case 0x448d10u:
        f->r[0]=restore_event_list_append_00448d10(f->r[1],A(1));cleanup=4;break;
    case 0x44d680u:
        f->r[0]=(f->r[0]&0xffffff00u)|restore_event_record_due_0044d680(f->r[1],A(1));
        cleanup=4;break;
    /* The flood handler and its scheduler return void. Their only native
       dispatcher continuations RET; the simulation loop then reloads EAX
       and replaces flags before testing. No volatile GPR output is consumed. */
    case 0x44e7b0u:
        restore_flood_event_apply_context(f->r[1],A(1),f);cleanup=4;break;
    case 0x4493c0u:
        f->r[0]=(f->r[0]&0xffffff00u)|FUN_004493c0((const RestoreTownRecord *)(uintptr_t)f->r[1]);break;
    case 0x40b5e0u:f->r[0]=FUN_0040b5e0((int32_t)A(1));break;
    case 0x40c180u:
        f->r[0]=(uint32_t)FUN_0040c180((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),
                                      (int32_t *)(uintptr_t)A(5),(int32_t *)(uintptr_t)A(6));break;
    case 0x417ae0u:FUN_00417ae0((int32_t)A(1));break;
    case 0x4425c0u:f->r[0]=FUN_004425c0(A(1));break;
    case 0x443000u:f->r[0]=FUN_00443000(A(1),A(2));break;
    case 0x443040u:f->r[0]=FUN_00443040(A(1),A(2));break;
    case 0x469c60u:FUN_00469c60((int32_t)A(1));break;
    case 0x47c2b0u:FUN_0047c2b0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
    case 0x47fe60u:f->r[0]=FUN_0047fe60((int32_t)A(1),(int32_t)A(2),A(3),(int32_t)A(4));break;
    case 0x4806c0u:f->r[0]=FUN_004806c0((int32_t)A(1),(int32_t)A(2),A(3));break;
    case 0x4b3250u:FUN_004b3250((int32_t)A(1));break;
    case 0x4b4800u:
        f->r[0]=(uint32_t)FUN_004b4800((uint8_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint8_t)A(5));break;
    case 0x4b5420u:FUN_004b5420((int32_t)A(1));break;
    case 0x4c7f30u:FUN_004c7f30();break;
    case 0x4cde40u:f->r[0]=(uint32_t)FUN_004cde40((int32_t)A(1),(int32_t)A(2));break;
    case 0x4cde60u:f->r[0]=(uint32_t)FUN_004cde60((int32_t)A(1),(int32_t)A(2));break;
    case 0x4e0cf0u:
        FUN_004e0cf0((int32_t)A(1),A(2),A(3),(const int16_t *)(uintptr_t)A(4),(uint8_t)A(5));break;
    case 0x4e96c0u:FUN_004e96c0((int32_t)A(1));break;
    case 0x4ea9c0u:f->r[0]=(uint32_t)FUN_004ea9c0((int32_t)A(1));break;
    case 0x516f90u:FUN_00516f90((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint8_t)A(5));break;
    case 0x5173a0u:FUN_005173a0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4));break;
    case 0x51f780u:break; /* Genuine single RET, preserving GPRs/flags. */
    case 0x53b090u:FUN_0053b090((int32_t)A(1));break;
    case 0x5596d0u:FUN_005596d0();break;
    case 0x5599c0u:f->r[0]=restore_economy_trade_validate_005599c0(A(1));break;
    case 0x55f449u:
        f->r[0]=FUN_0055f449((RestoreSound *)(uintptr_t)f->r[1],(const char *)(uintptr_t)A(1),
                           (int32_t)A(2),A(3));cleanup=12;break;
    case 0x55f561u:FUN_0055f561((RestoreSound *)(uintptr_t)f->r[1],(int32_t)A(1));cleanup=4;break;
    case 0x55f5a1u:f->r[0]=FUN_0055f5a1((RestoreSound *)(uintptr_t)f->r[1],(int32_t)A(1));cleanup=4;break;
    case 0x56133cu:{
        uint8_t raw80[10];uint64_t value;
        __asm { lea eax,raw80 }
        __asm { fstp tbyte ptr[eax] }
        value=restore_menu_input_trunc80(raw80);
        f->r[0]=(uint32_t)value;f->r[2]=(uint32_t)(value>>32);break;
    }
    case 0x561a36u:
        f->r[0]=(uint32_t)FUN_00561a36((char *)(uintptr_t)A(1),(const char *)(uintptr_t)A(2),
                                     (int32_t)A(3),(int32_t)A(4),(int32_t)A(5),(int32_t)A(6));break;
    default:abort();
    }
    /* Typed logical providers publish only the outputs consumed by their
       actual call continuations. AL predicates keep that narrow contract.
       Native members retain ECX and RET4/RET12; ordinary cdecl callers own
       their argument slots. Raw families share the exact same native stack.
       Hardware x87 values/CW use the accepted masked-exception contract;
       exception traps and FPSW equivalence are not claimed. */
    f->r[4]=sp+4u+cleanup;
#undef RAW
#undef A
}

void restore_event_actions_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_event_actions_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
    dispatch(0,target,&f);
    memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

static uint32_t invoke_words(uint32_t entry,uint32_t self,const uint32_t *words,uint32_t count,uint32_t cleanup)
{
    uint32_t stack[16384],sp,i;
    struct restore_event_actions_frame f={{0},2,dispatch};
    /* Callback inputs are globals, not an incoming receiver or FPU value.
       Native record/member receivers are set by the actual instructions.
       Keep natural scratch and shared lower-stack history throughout the
       recursive event graph; no dummy output values or recursion clamp. */
    sp=(uint32_t)(uintptr_t)(stack+16000);stack[16000]=0;f.r[4]=sp;f.r[1]=self;
    for(i=0;i<count;++i)stack[16001u+i]=words[i];
    dispatch(0,entry,&f);
    if(f.r[4]!=sp+4u+cleanup)abort();
    return f.r[0];
}

static void invoke(uint32_t entry,uint32_t self)
{(void)invoke_words(entry,self,0,0,0);}

/* Reuse the current event graph's stack for nested logical providers. A
   fresh64KB arena at every immediate follow-up would grow the host stack
   much faster than the original. Reserve the flood handler's48-byte call
   area below its native entry; modeled descendants own the lower area. */
static uint32_t invoke_child_words(uint32_t entry,uint32_t self,const uint32_t *words,
    uint32_t count,uint32_t cleanup,struct restore_event_actions_frame *parent)
{
    struct restore_event_actions_frame child;uint32_t sp,i;
    if(!parent)return invoke_words(entry,self,words,count,cleanup);
    child=*parent;sp=parent->r[4]-48u;child.r[4]=sp;child.r[1]=self;child.boundary=dispatch;
    U(sp)=0;for(i=0;i<count;++i)U(sp+4u+i*4u)=words[i];
    dispatch(0,entry,&child);
    if(child.r[4]!=sp+4u+cleanup)abort();
    return child.r[0];
}
uint32_t restore_event_graph_query_context(uint32_t target,uint32_t self,int32_t argument,struct restore_event_actions_frame *context)
{
    uint32_t word=(uint32_t)argument,count;
    switch(target){
    case 0x449390u:case 0x451540u:case 0x44c660u:count=0;break;
    case 0x449550u:count=1;break;
    default:abort();
    }
    return invoke_child_words(target,self,&word,count,count*4u,context);
}
int32_t restore_event_followup_message_context(uint32_t self,uint32_t slot,struct restore_event_actions_frame *context)
{return (int32_t)invoke_child_words(0x44fd30u,self,&slot,1,4,context);}
int32_t restore_event_followup_message(uint32_t self,uint32_t slot)
{return restore_event_followup_message_context(self,slot,0);}
void restore_event_schedule_context(uint32_t self,int32_t template_id,int16_t reason,struct restore_event_actions_frame *context)
{
    uint32_t words[2]={(uint32_t)template_id,(uint32_t)(int32_t)reason};
    (void)invoke_child_words(0x44c490u,self,words,2,8,context);
}

/* Simulation4464F0 supplies a live124-byte record. Share the complete event
   member graph with the menu so all descendants retain their ECX receivers,
   native stack locals and RETn contracts (including44E6F0's RET4). */
void restore_event_update_00451540(RestoreTownRecord *self)
{invoke(0x451540u,(uint32_t)(uintptr_t)self);}

/* The explicit scheduling API shares the event stack with its descendants. */
void restore_event_schedule_0044c490(RestoreTownRecord *self,int32_t template_id,int16_t reason)
{
    uint32_t words[2]={(uint32_t)template_id,(uint32_t)(int32_t)reason};
    (void)invoke_words(0x44c490u,(uint32_t)(uintptr_t)self,words,2,8);
}

/* Use the same complete 142/38-instruction bodies as the event graph.
   4E7C90 defines AL only; retain its native upper EAX bits at this facade. */
int32_t FUN_004e7ac0(int32_t duration)
{return (int32_t)restore_spawn_patrol_at_random_building(duration);}
uint32_t FUN_004e7c90(int32_t building)
{return restore_building_patrol_eligibility_result(building);}

int restore_menu_event_action(uint32_t token)
{
    if(token!=0x44ee30u&&token!=0x44eea0u&&token!=0x44eef0u)return 0;
    invoke(token,0);
    return 1;
}

/* The sole actual44BA90 caller4523FB explicitly sets ECX=785D70.
   No native E9 or absolute-address references. Keep its existing logical
   nullary C interface while providing the real simulation manager. */
void FUN_0044ba90(void){invoke(0x44ba90u,0x785d70u);}

/* Whole448960/27 instructions. Native ECX is explicit. Constructor stores
   precede4489C0; these are not a memset of the124-byte event record. */
static uint32_t construct_event(uint32_t self)
{
    volatile uint16_t *record=(volatile uint16_t *)(uintptr_t)self;
    record[3]=0xffff;record[5]=0xffff;record[4]=0xffff;record[2]=0xffff;
    record[7]=0xffff;record[9]=0xffff;record[8]=0xffff;record[6]=0xffff;
    record[11]=0xffff;record[13]=0xffff;record[12]=0xffff;record[10]=0xffff;
    record[15]=0xffff;record[17]=0xffff;record[16]=0xffff;record[14]=0xffff;
    record[45]=0xffff;record[47]=0xffff;record[46]=0xffff;record[44]=0xffff;
    restore_event_record_reset_004489c0(self);
    return self;
}

/* Whole44BA20/29 instructions, normalized ECX+self, native RET0/EAX=self.
   At this561441 vector-constructor site, all150 constructors contain only
   field stores and the genuine reset; the supplied destructor is C3.
   The normal nonthrowing path is a C loop with the same element order.
   No generic native callback addresses or SEH-unwind equivalence implied. */
uint32_t FUN_0044ba20(uint32_t self)
{
    uint32_t index,record=self+4u;
    for(index=0;index<150u;++index){(void)construct_event(record);record+=124u;}
    U(self)=0;U(self+0x48acu)=UINT32_MAX;U(self+0x48b0u)=0;
    record=self+4u;
    for(index=0;index<150u;++index){
        *(volatile uint16_t *)(uintptr_t)record=(uint16_t)index;record+=124u;
    }
    restore_town_actor_links_00448a60((void *)(uintptr_t)(self+0x43d4u));
    *(volatile uint8_t *)(uintptr_t)(self+0x43d6u)=2;
    *(volatile uint8_t *)(uintptr_t)(self+0x48b4u)=0;
    *(volatile uint8_t *)(uintptr_t)(self+0x48b5u)=0;
    return self;
}

/* Actual4483A0 is MOV ECX,785D70; JMP44BA20. Existing C caller448390
   discards EAX before its atexit registration. */
void FUN_004483a0(void){(void)FUN_0044ba20(0x785d70u);}

uint32_t restore_event_construct_00448960(uint32_t self)
{return construct_event(self);}
