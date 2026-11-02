#include "editor_event_detail_actions.h"
#include "numeric_input.h"
#include "selection_popup.h"
#include <stdlib.h>
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x4498f017)
typedef char event_detail_pointer32[sizeof(void *)==4?1:-1];
/* The whole internal integer graph shares one explicit native stack/register
   frame. This retains partial AL and the packed three-WORD local ranges.
   Only actual reviewed C bodies are dispatched; no original code is executed. */
static void dispatch(uint32_t site,uint32_t target,struct restore_event_detail_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
 switch(target){
#define RAW(a) case 0x##a##u:restore_event_detail_C_##a(f);return;
 RAW(004493b0) RAW(004493d0) RAW(004493e0) RAW(004493f0)
 RAW(00449440) RAW(00449530) RAW(00449550) RAW(004495c0)
 RAW(004495d0) RAW(004495f0) RAW(00449600) RAW(00449630)
 RAW(00449640) RAW(00449660) RAW(00449690) RAW(004496b0)
 RAW(00449710) RAW(00449870) RAW(004498b0) RAW(004498f0)
 RAW(004499c0) RAW(00449b00) RAW(00449b30) RAW(00449b60)
 RAW(00449bf0) RAW(00449c20) RAW(00449de0) RAW(00449f90)
 RAW(0044a140) RAW(0044a2f0) RAW(0044a3d0) RAW(0044a470)
 RAW(0044a4f0) RAW(0044a560) RAW(0044a5e0) RAW(0044a670)
 RAW(0044a6c0) RAW(0044a6f0) RAW(0044a760) RAW(0044a790)
 RAW(0044a800) RAW(0044a830) RAW(0044a8a0) RAW(0044a8d0)
 RAW(0044a940) RAW(0044aa30)
#undef RAW
 }
#define A(n) U(sp+(uint32_t)(n)*4u)
 switch(target){
 case 0x403870u:FUN_00403870(A(1),A(2),A(3),A(4));break;
 case 0x403c00u:
  FUN_00403c00(A(1),A(2),(int32_t)A(3),A(4),A(5),A(6),A(7),A(8),
               A(9),A(10),A(11),A(12),A(13),(int32_t)A(14),A(15));break;
 default:abort();
 }
 /* Both genuine modal providers are native RET0. Their volatile outputs
    are dead at every owned external callsite; subsequent stack arithmetic
    is still modeled. Internal calls retain actual GPR/flags and RET cleanup. */
 f->r[4]=sp+4u;
#undef A
}
void restore_event_detail_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 struct restore_event_detail_frame f;
 memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=dispatch;
 dispatch(0,target,&f);
 memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

static void invoke(uint32_t target)
{
 /* Static native CFG/call-depth audit: all46 return paths balance, no
    recursion, minimum combined virtual ESP is entry-76 bytes. The two
    packed range callbacks read DWORDs containing three defined WORDs;
    unused upper halves never feed a decision, call argument or game store. */
 uint32_t stack[512],entry,i;struct restore_event_detail_frame f;
 for(i=0;i<512;++i)stack[i]=SENTRY;
 for(i=0;i<8;++i)f.r[i]=0;
 entry=(uint32_t)(uintptr_t)(stack+384);
 f.r[4]=entry;f.flags=2;f.boundary=dispatch;
 dispatch(0,target,&f);
 if(f.r[4]!=entry+4u||stack[384]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=SENTRY)abort();
 for(i=385;i<512;++i)if(stack[i]!=SENTRY)abort();
}
void restore_editor_event_detail_004498f0(void){invoke(0x4498f0u);}
void restore_editor_event_detail_00449b60(void){invoke(0x449b60u);}
void restore_editor_event_detail_00449c20(void){invoke(0x449c20u);}
void restore_editor_event_detail_00449de0(void){invoke(0x449de0u);}
void restore_editor_event_detail_00449f90(void){invoke(0x449f90u);}
void restore_editor_event_detail_0044a140(void){invoke(0x44a140u);}
void restore_editor_event_detail_0044a2f0(void){invoke(0x44a2f0u);}
void restore_editor_event_detail_0044a3d0(void){invoke(0x44a3d0u);}
void restore_editor_event_detail_0044a470(void){invoke(0x44a470u);}
void restore_editor_event_detail_0044a4f0(void){invoke(0x44a4f0u);}
void restore_editor_event_detail_0044a560(void){invoke(0x44a560u);}
void restore_editor_event_detail_0044a5e0(void){invoke(0x44a5e0u);}
void restore_editor_event_detail_0044a670(void){invoke(0x44a670u);}
void restore_editor_event_detail_0044a6f0(void){invoke(0x44a6f0u);}
void restore_editor_event_detail_0044a790(void){invoke(0x44a790u);}
void restore_editor_event_detail_0044a830(void){invoke(0x44a830u);}
void restore_editor_event_detail_0044a8d0(void){invoke(0x44a8d0u);}
void restore_editor_event_detail_0044aa30(void){invoke(0x44aa30u);}
void restore_editor_event_completion_00449870(void){invoke(0x449870u);}
void restore_editor_event_completion_004498b0(void){invoke(0x4498b0u);}
void restore_editor_event_completion_004499c0(void){invoke(0x4499c0u);}
void restore_editor_event_completion_00449bf0(void){invoke(0x449bf0u);}
void restore_editor_event_completion_0044a6c0(void){invoke(0x44a6c0u);}
void restore_editor_event_completion_0044a760(void){invoke(0x44a760u);}
void restore_editor_event_completion_0044a800(void){invoke(0x44a800u);}
void restore_editor_event_completion_0044a8a0(void){invoke(0x44a8a0u);}
void restore_editor_event_completion_0044a940(void){invoke(0x44a940u);}
/* Only the eighteen actual main table5D9E20/count30 identities. */
int restore_editor_event_detail_action(uint32_t token)
{
 switch(token){
 case 0x4498f0u:restore_editor_event_detail_004498f0();return 1;
 case 0x449b60u:restore_editor_event_detail_00449b60();return 1;
 case 0x449c20u:restore_editor_event_detail_00449c20();return 1;
 case 0x449de0u:restore_editor_event_detail_00449de0();return 1;
 case 0x449f90u:restore_editor_event_detail_00449f90();return 1;
 case 0x44a140u:restore_editor_event_detail_0044a140();return 1;
 case 0x44a2f0u:restore_editor_event_detail_0044a2f0();return 1;
 case 0x44a3d0u:restore_editor_event_detail_0044a3d0();return 1;
 case 0x44a470u:restore_editor_event_detail_0044a470();return 1;
 case 0x44a4f0u:restore_editor_event_detail_0044a4f0();return 1;
 case 0x44a560u:restore_editor_event_detail_0044a560();return 1;
 case 0x44a5e0u:restore_editor_event_detail_0044a5e0();return 1;
 case 0x44a670u:restore_editor_event_detail_0044a670();return 1;
 case 0x44a6f0u:restore_editor_event_detail_0044a6f0();return 1;
 case 0x44a790u:restore_editor_event_detail_0044a790();return 1;
 case 0x44a830u:restore_editor_event_detail_0044a830();return 1;
 case 0x44a8d0u:restore_editor_event_detail_0044a8d0();return 1;
 case 0x44aa30u:restore_editor_event_detail_0044aa30();return 1;
 default:return 0;
 }
}
/* Actual selection403C00 completions, required for a complete legal click.
   The seven numeric403870 completions already belong to numeric_completion. */
int restore_editor_event_detail_completion_action(uint32_t token)
{
 switch(token){
 case 0x449870u:restore_editor_event_completion_00449870();return 1;
 case 0x4498b0u:restore_editor_event_completion_004498b0();return 1;
 case 0x4499c0u:restore_editor_event_completion_004499c0();return 1;
 case 0x449bf0u:restore_editor_event_completion_00449bf0();return 1;
 case 0x44a6c0u:restore_editor_event_completion_0044a6c0();return 1;
 case 0x44a760u:restore_editor_event_completion_0044a760();return 1;
 case 0x44a800u:restore_editor_event_completion_0044a800();return 1;
 case 0x44a8a0u:restore_editor_event_completion_0044a8a0();return 1;
 case 0x44a940u:restore_editor_event_completion_0044a940();return 1;
 default:return 0;
 }
}
