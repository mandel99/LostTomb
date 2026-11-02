#include "remaining_editor_list_actions.h"
#include "numeric_input.h"
#include "selection_popup.h"
#include "map_validation.h"
#include "menu_dialog.h"
#include "monument_menu_render.h"
#include <stdlib.h>
/* Genuine accepted native MOV EAX,38;RET, not an unresolved-result stub. */
extern uint32_t FUN_00426b30(void);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x40c92017)
typedef char remaining_editor_pointer32[sizeof(void *)==4?1:-1];
static void dispatch(uint32_t site,uint32_t target,struct restore_remaining_editor_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
 switch(target){
#define RAW(a) case 0x##a##u:restore_remaining_editor_C_##a(f);return;
 RAW(0040c8a0) RAW(0040c920)
 RAW(0040cbe0) RAW(0040cc40) RAW(0040cc80)
#undef RAW
 }
#define A(n) U(sp+(uint32_t)(n)*4u)
 switch(target){
 case 0x40cb40u:{
  RestoreMonumentMenuOption option;
  option.words[0]=A(2);option.words[1]=A(3);
  option.words[2]=A(4);option.words[3]=A(5);
  f->r[0]=restore_monument_option_allowed_0040cb40(A(1),option);break;
 }
 case 0x426b30u:f->r[0]=FUN_00426b30();break;
 case 0x40c8b0u:f->r[0]=FUN_0040c8b0((int32_t)A(1));break;
 case 0x442870u:f->r[0]=FUN_00442870(A(1));break;
 case 0x4264d0u:FUN_004264d0(A(1),A(2),A(3));break;
 case 0x403790u:FUN_00403790(A(1),A(2),A(3),A(4),A(5),A(6),A(7));break;
 case 0x403c00u:
  FUN_00403c00(A(1),A(2),(int32_t)A(3),A(4),A(5),A(6),A(7),A(8),
               A(9),A(10),A(11),A(12),A(13),(int32_t)A(14),A(15));break;
 default:abort();
 }
 /* All seven external targets are native RET0. The four query returns are
    consumed as actual full EAX count or AL predicates; ECX/EDX/flags and
    incidental high EAX predicate bits are dead before any later definition. */
 f->r[4]=sp+4u;
#undef A
}
static void invoke(uint32_t target)
{
 /* All five native CFGs balance RET0; maximum composed virtual stack is
   100 bytes. The by-value option has all sixteen bytes defined by the four
   original DWORD copies; no retired scratch or guessed zero bytes are inputs. */
 uint32_t stack[512],entry,i;struct restore_remaining_editor_frame f;
 for(i=0;i<512;++i)stack[i]=SENTRY;
 for(i=0;i<8;++i)f.r[i]=0;
 entry=(uint32_t)(uintptr_t)(stack+384);
 f.r[4]=entry;f.flags=2;f.boundary=dispatch;
 dispatch(0,target,&f);
 if(f.r[4]!=entry+4u||stack[384]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=SENTRY)abort();
 for(i=385;i<512;++i)if(stack[i]!=SENTRY)abort();
}
void restore_remaining_editor_0040c8a0(void){invoke(0x40c8a0u);}
void restore_remaining_editor_0040c920(void){invoke(0x40c920u);}
void restore_remaining_editor_0040cbe0(void){invoke(0x40cbe0u);}
void restore_remaining_editor_0040cc80(void){invoke(0x40cc80u);}
uint32_t restore_remaining_editor_count_0040cc40(uint32_t phase)
{
 uint32_t stack[64],sp=(uint32_t)(uintptr_t)(stack+48);
 struct restore_remaining_editor_frame f={{0},2,dispatch};
 stack[48]=0;stack[49]=phase;f.r[4]=sp;dispatch(0,0x40cc40u,&f);
 return f.r[0];
}
int restore_remaining_editor_list_action(uint32_t token)
{
 switch(token){
 case 0x40c8a0u:restore_remaining_editor_0040c8a0();return 1;
 case 0x40c920u:restore_remaining_editor_0040c920();return 1;
 case 0x40cbe0u:restore_remaining_editor_0040cbe0();return 1;
 case 0x40cc80u:restore_remaining_editor_0040cc80();return 1;
 default:return 0;
 }
}
