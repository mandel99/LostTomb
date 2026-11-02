#include "numeric_input.h"
#include "menu_input.h"
#include "map_file_load.h"
#include "city_prepare.h"
#include "redraw_dispatch_model.h"
#include "host_allocator.h"
#include "city_numeric_confirm.h"
extern uint32_t FUN_0047d350(void);
#include <stdlib.h>
/* Actual source read service already shared by the sprite reader. Its file /
   device retry UI remains external to this numeric/bitmap body translation. */
extern int FUN_004ceb60(const char *path,void *destination,int32_t offset);
/* Mode2 is map_cells' separately owned whole provider, never old unit634. */
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x4035f017)
typedef char numeric_input_pointer32[sizeof(void *)==4?1:-1];

static void dispatch(uint32_t site,uint32_t target,struct restore_numeric_frame *f)
{
 uint32_t sp=f->r[4];
 /* Native CALL EAX receives no explicit argument. Route the actual token;
    do not cast original code, clear it early, or cache the close-mode byte. */
 if(site==0x403732u){restore_menu_callback(target);f->r[4]=sp+4u;return;}
 switch(target){
#define RAW(a) case 0x##a##u:restore_numeric_C_##a(f);return;
 RAW(004035a0) RAW(004035f0) RAW(00403770) RAW(00403790)
 RAW(00403800) RAW(00403870) RAW(004038e0)
 RAW(004cede0) RAW(004cee00) RAW(004de160) RAW(004de200)
#undef RAW
 }
#define A(n) U(sp+(uint32_t)(n)*4u)
 switch(target){
 case 0x47d350u:f->r[0]=FUN_0047d350();break;
 case 0x444270u:restore_city_numeric_00444270((int32_t)A(1));break;
 case 0x4cbea0u:FUN_004cbea0(A(1),(char *)(uintptr_t)A(2),(int32_t)A(3));break;
 case 0x561b68u:f->r[0]=(uint32_t)(uintptr_t)restore_heap_allocate(A(1));break;
 case 0x561bdcu:restore_heap_free((void *)(uintptr_t)A(1));break;
 case 0x4ceb60u:f->r[0]=(uint32_t)FUN_004ceb60((const char *)(uintptr_t)A(1),(void *)(uintptr_t)A(2),(int32_t)A(3));break;
 case 0x46fa40u:FUN_0046fa40();break;
 case 0x4dea50u:FUN_004dea50();break;
 default:abort();
 }
 /* Each external call here is RET0. Volatile return registers/flags other
    than the declared EAX result are dead before their next definition. */
 f->r[4]=sp+4u;
#undef A
}
static void invoke(uint32_t target,const uint32_t *args,uint32_t count)
{
 /* Native owned CFG minima including nested raw calls: confirm -120B,
    source loader -112B, bitmap flip -96B. Typed shared providers use their
    own C stack. Every owned local buffer read is initialized by real copies. */
 uint32_t stack[512],entry,i;struct restore_numeric_frame f;
 for(i=0;i<512;++i)stack[i]=SENTRY;
 for(i=0;i<8;++i)f.r[i]=0;
 entry=(uint32_t)(uintptr_t)(stack+384);
 for(i=0;i<count;++i)stack[385+i]=args[i];
 f.r[4]=entry;f.flags=2;f.boundary=dispatch;dispatch(0,target,&f);
 if(f.r[4]!=entry+4u||stack[384]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<32;++i)if(stack[i]!=SENTRY)abort();
 for(i=385+count;i<512;++i)if(stack[i]!=SENTRY)abort();
}
void FUN_004035a0(void){invoke(0x4035a0u,0,0);}
void FUN_004035f0(void){invoke(0x4035f0u,0,0);}
void FUN_00403770(void){invoke(0x403770u,0,0);}
void FUN_00403790(uint32_t x,uint32_t y,uint32_t width,uint32_t byte_target,
                  uint32_t word_target,uint32_t dword_target,uint32_t mode)
{uint32_t a[]={x,y,width,byte_target,word_target,dword_target,mode};invoke(0x403790u,a,7);}
void FUN_00403870(uint32_t x,uint32_t y,uint32_t word_target,uint32_t completion)
{uint32_t a[]={x,y,word_target,completion};invoke(0x403870u,a,4);}
void restore_numeric_open_dword_00403800(uint32_t x,uint32_t y,uint32_t target,uint32_t completion)
{uint32_t a[]={x,y,target,completion};invoke(0x403800u,a,4);}
void restore_numeric_open_byte_004038e0(uint32_t x,uint32_t y,uint32_t target,uint32_t completion)
{uint32_t a[]={x,y,target,completion};invoke(0x4038e0u,a,4);}
void restore_numeric_source_004de160(int32_t index)
{uint32_t a[]={(uint32_t)index};invoke(0x4de160u,a,1);}
int restore_numeric_input_action(uint32_t token)
{
 switch(token){
 case 0x4035a0u:FUN_004035a0();return 1;
 case 0x4035f0u:FUN_004035f0();return 1;
 case 0x403770u:FUN_00403770();return 1;
 default:return 0;
 }
}
