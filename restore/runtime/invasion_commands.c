#include "invasion_commands.h"
#include "invasion_model.h"
#include "actor_callbacks_human.h"
#include "actor_create.h"
#include "actor_profile_model.h"
#include "actor_walking.h"
#include "building_footprint.h"
#include "campaign_score_numeric.h"
#include "canal_region_model.h"
#include "construction_refresh.h"
#include "event_publisher.h"
#include "god_building_effects.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "menu_list_scroll.h"
#include "menu_prepare_integer.h"
#include "mission_unlock.h"
#include "placement_support.h"
#include "remaining_menu_actions.h"
#include <stdlib.h>
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))
typedef char invasion_pointer32[sizeof(void *)==4?1:-1];
extern uint32_t FUN_00474380(int32_t kind);
extern int32_t FUN_004b6ff0(uint16_t kind,int32_t nation,int32_t entry,uint16_t group,uint16_t flags);

/* Whole 45A0A0, 58 instructions. Three lists of seven DWORDs produce a
   signed32 wrapped divisor. Preserve FILD32/FIDIV32/FIMUL32 and extended
   precision until the genuine FTOL conversion; no binary64 intermediate.
   The caller's third argument is native scratch, dead at all six 45A140
   sites. This typed API exposes the consumed low32 result, not dead EDX or
   that discarded argument slot. Inherited masked PC/RC; FPSW not an API. */
static uint32_t weighted_value(int32_t amount,int32_t category,uint32_t table)
{
    uint32_t sums[3],group,denominator,term;
    int32_t numerator,divisor;
    uint8_t raw80[10];
    if(amount<=0)return 0;
    for(group=0;group<3u;++group){
        uint32_t list=table+group*28u,left=U(list),sum=0;
        if((int32_t)left>0){
            list+=4u;
            do{
                uint32_t resource=U(list);list+=4u;
                sum+=U(0x705af0u+resource*4u);
            }while(--left);
        }
        sums[group]=sum;
    }
    denominator=U(table+80u)*sums[2];
    term=U(table+52u)*sums[1];denominator+=term;
    term=U(table+24u)*sums[0];denominator+=term;
    if(!denominator)return 0;
    numerator=(int32_t)U(table+(uint32_t)category*28u+24u);
    divisor=(int32_t)denominator;
    __asm {
        fild numerator
        fidiv divisor
        fimul amount
        lea eax,raw80
        fstp tbyte ptr[eax]
    }
    return (uint32_t)restore_campaign_score_trunc80(raw80);
}

/* Whole 45A140, 70 instructions; seven-target remap at 45A250/45A26C. */
static uint32_t building_value(int32_t building)
{
    uint32_t record=(uint32_t)building*264u,offset;
    int32_t kind,category;
    if(B(0x7999d8u+record)!=1u)return 0;
    kind=S16(0x7999e8u+record);
    switch(kind){
    case 77:category=0;break;
    case 78:category=1;break;
    case 79:category=2;break;
    case 84:case 85:case 187:case 188:case 189:
        offset=U(0xeaac28u)*0x49d8u;
        return weighted_value((int32_t)U(0xea187cu+offset),2,0x5da4f0u);
    case 86:case 87:
        offset=U(0xeaac28u)*0x49d8u;
        return weighted_value((int32_t)U(0xea187cu+offset),0,0x5da4f0u);
    case 184:
        offset=U(0xeaac28u)*0x49d8u;
        return weighted_value((int32_t)U(0xea187cu+offset),1,0x5da4f0u);
    default:return 0;
    }
    offset=U(0xeaac28u)*0x49d8u;
    return weighted_value((int32_t)U(0xea5cc0u+offset),category,0x5da498u);
}

/* All model and external entries below are native RET0. New models retain
   their real locals, output aliases and GPR results. The typed providers'
   discarded volatile registers/flags are not fabricated as return values. */
static void dispatch(uint32_t site,uint32_t target,struct restore_invasion_frame *f)
{
    uint32_t sp=f->r[4];
    (void)site;
#define A(n) U(sp+4u*(n))
#define RAW(address) case 0x##address##u:restore_invasion_C_##address(f);return
    switch(target){
    RAW(00446a30);RAW(004467a0);RAW(00446650);RAW(004620c0);RAW(00469140);
    RAW(004b7510);RAW(00459ec0);RAW(004c03d0);RAW(004684e0);RAW(0048c0e0);
    RAW(00497b90);RAW(00480060);RAW(004800b0);
    case 0x43fd00u:case 0x4695d0u:case 0x46d0f0u:
        restore_god_building_raw(target,f->r,&f->flags);return;
    case 0x459ef0u:f->r[0]=FUN_00459ef0((int32_t)A(1));break;
    case 0x45a140u:f->r[0]=building_value((int32_t)A(1));break;
    case 0x474380u:{uint32_t a=A(1);f->r[0]=(a&0xffffff00u)|(uint8_t)FUN_00474380((int32_t)a);break;}
    case 0x475b50u:f->r[0]=FUN_00475b50(A(1));break;
    case 0x47afd0u:FUN_0047afd0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3));break;
    case 0x47bae0u:FUN_0047bae0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),A(5));break;
    case 0x47c2b0u:FUN_0047c2b0((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(int32_t)A(5));break;
    case 0x4b3250u:FUN_004b3250((int32_t)A(1));break;
    case 0x4b4220u:FUN_004b4220((int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),
                              (int32_t)A(5),(int32_t)A(6));break;
    case 0x4b4800u:f->r[0]=(uint32_t)FUN_004b4800((uint8_t)A(1),(int32_t)A(2),(int32_t)A(3),
                                                (int32_t)A(4),(uint8_t)A(5));break;
    case 0x4b52f0u:FUN_004b52f0((int32_t)A(1));break;
    case 0x4b5420u:FUN_004b5420((int32_t)A(1));break;
    case 0x4b5af0u:FUN_004b5af0((int32_t)A(1));break;
    case 0x4b6ff0u:f->r[0]=(uint32_t)FUN_004b6ff0((uint16_t)A(1),(int32_t)A(2),(int32_t)A(3),
                                                (uint16_t)A(4),(uint16_t)A(5));break;
    case 0x4cde40u:f->r[0]=(uint32_t)FUN_004cde40((int32_t)A(1),(int32_t)A(2));break;
    case 0x4e2970u:restore_remaining_message_4e2970((int32_t)A(1),A(2),A(3));break;
    case 0x517370u:FUN_00517370();break;
    case 0x517660u:FUN_00517660();break;
    case 0x5178e0u:FUN_005178e0();break;
    case 0x53b090u:FUN_0053b090((int32_t)A(1));break;
    default:abort();
    }
#undef RAW
#undef A
    f->r[4]=sp+4u;
}

void restore_invasion_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
    struct restore_invasion_frame f;
    memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
    dispatch(0,target,&f);
    memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

uint32_t FUN_004467a0(int32_t *x,int32_t *y)
{
    uint32_t stack[64],sp=(uint32_t)(uintptr_t)(stack+48);
    struct restore_invasion_frame f={{0},2,dispatch};
    stack[48]=0;stack[49]=(uint32_t)(uintptr_t)x;stack[50]=(uint32_t)(uintptr_t)y;
    f.r[4]=sp;
    restore_invasion_C_004467a0(&f);
    if(f.r[4]!=sp+4u)abort();
    return (uint8_t)f.r[0];
}

uint32_t restore_invasion_00446a30(int32_t nation,int32_t count,int32_t entry,
                                  uint32_t argument3,int32_t event_index)
{
    /* Native locals remain uninitialized until their actual instructions
       write them. The composed raw stack floor is entry-648, including the
       accepted God graph's conservative448-byte allowance; reserve1536.
       Incoming volatile GPRs are dead, and incoming nonvolatiles are only
       saved/restored. RET0 leaves the five argument slots caller-owned. */
    uint32_t stack[512],sp=(uint32_t)(uintptr_t)(stack+384);
    struct restore_invasion_frame f={{0},2,dispatch};
    stack[384]=0;
    stack[385]=(uint32_t)nation;stack[386]=(uint32_t)count;
    stack[387]=(uint32_t)entry;stack[388]=argument3;stack[389]=(uint32_t)event_index;
    f.r[4]=sp;
    restore_invasion_C_00446a30(&f);
    if(f.r[4]!=sp+4u)abort();
    return f.r[0];
}

static void command(int32_t count,int32_t entry)
{
    int32_t nation=S16(0x7848fcu);
    uint32_t result=restore_invasion_00446a30(nation,count,entry,0,140);
    if(result){
        int32_t message=S16(0x7848fcu)<12?0x7a:0x139,counter;
        U(0xe950dcu)=1;counter=S16(0x775900u);
        FUN_004e0cf0(message,(uint32_t)counter,result,0,0);
    }
}

void restore_invasion_command_00446530(void){command(15,8);}
void restore_invasion_command_004465a0(void){command(16,9);}
int restore_invasion_command_action(uint32_t token)
{
    switch(token){
    case 0x446530u:restore_invasion_command_00446530();return 1;
    case 0x4465a0u:restore_invasion_command_004465a0();return 1;
    default:return 0;
    }
}
