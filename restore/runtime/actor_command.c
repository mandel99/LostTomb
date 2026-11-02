#include "actor_command.h"
#include "actor_create.h"
#include "actor_combat.h"
#include "editor_validation.h"
#include "building_nearest_search.h"
#include <stdlib.h>
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x41cd0a17)
typedef char actor_command_pointer32[sizeof(void*)==4?1:-1];
extern int32_t FUN_004eada0(int32_t kind);
/* Audited whole native45-instruction leaf; low WORD kind/X/Y consumed. */
extern int32_t FUN_004b6ff0(uint16_t kind,int32_t mode,int32_t orientation,uint16_t x,uint16_t y);

static void dispatch(uint32_t site,uint32_t target,struct restore_actor_command_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
 switch(target){
#define RAW(a) case 0x##a##u:restore_actor_command_C_##a(f);return;
 RAW(0041cd00) RAW(004ec990) RAW(004ec0f0)
 RAW(004ec7c0) RAW(004ec4a0) RAW(004eae20)
#undef RAW
 }
#define A(n) U(sp+(uint32_t)(n)*4u)
 switch(target){
 case 0x4b4800u:
  f->r[0]=(uint32_t)FUN_004b4800((uint8_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint8_t)A(5));break;
 case 0x4b6ff0u:
  f->r[0]=(uint32_t)FUN_004b6ff0((uint16_t)A(1),(int32_t)A(2),(int32_t)A(3),(uint16_t)A(4),(uint16_t)A(5));break;
 case 0x4ec410u:f->r[0]=FUN_004ec410(A(1),(char)A(2));break;
 case 0x4ea9c0u:f->r[0]=(uint32_t)FUN_004ea9c0((int32_t)A(1));break;
 case 0x4eada0u:f->r[0]=(uint32_t)FUN_004eada0((int32_t)A(1));break;
 case 0x4eb800u:
  /* Actual native9-DWORD/RET0 ABI, no ECX. The caller's output buffers are
     VM-stack words, disjoint from map/type data, exactly its accepted domain. */
  f->r[0]=(uint32_t)restore_building_nearest_search(
   (int32_t)A(1),(int32_t)A(2),(int32_t)A(3),(const uint32_t *)(uintptr_t)A(4),A(5),
   (int32_t *)(uintptr_t)A(6),(int32_t *)(uintptr_t)A(7),(int32_t)A(8),(int32_t)A(9));break;
 default:abort();
 }
 f->r[4]=sp+4u; /* Every genuine external target here is native RET0. */
#undef A
}
static uint32_t invoke(uint32_t target,const uint32_t *args,uint32_t count)
{
 uint32_t stack[512],entry,i;struct restore_actor_command_frame f;
 for(i=0;i<512;++i)stack[i]=SENTRY;
 for(i=0;i<8;++i)f.r[i]=0;
 /* Command41CF7C writes actor state25. EC7C0's state25 weights50/50
    select only table5E4E70 entries0/1 =19/18, both nonrecursive EC4A0
    branches. Thus the command-owned raw stack floor is -172 bytes.
    For a valid generic EC990 actor state, only the default weights can
    choose table entry3=25; that permits at most one backedge, floor-208.
    No state clamp or artificial iteration cutoff implements this invariant. */
 entry=(uint32_t)(uintptr_t)(stack+384);
 for(i=0;i<count;++i)stack[385+i]=args[i];
 f.r[4]=entry;f.flags=2;f.boundary=dispatch;dispatch(0,target,&f);
 if(f.r[4]!=entry+4u||stack[384]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=SENTRY)abort();
 for(i=385+count;i<512;++i)if(stack[i]!=SENTRY)abort();
 return f.r[0];
}
void restore_actor_command_0041cd00(void){invoke(0x41cd00u,0,0);}
void restore_actor_command_004ec990(int32_t actor)
{uint32_t a[]={(uint32_t)actor};invoke(0x4ec990u,a,1);}
uint32_t FUN_004ec4a0(int32_t actor,uint32_t state)
{const uint32_t a[]={(uint32_t)actor,state};return invoke(0x4ec4a0u,a,2);}

/* Immutable command table57C3C8 row3 names this genuine nullary callback.
   Its two native local DWORDs are written before use; none of the six newly
   owned bodies reads a30-byte payload or retired decoder storage. */
int restore_actor_command_action(uint32_t token)
{
 if(token!=0x41cd00u)return 0;
 restore_actor_command_0041cd00();return 1;
}

/* Preserve the complete native frame when animal callbacks call this graph. */
void restore_actor_command_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 struct restore_actor_command_frame f;
 memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
 dispatch(0,target,&f);
 memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}
