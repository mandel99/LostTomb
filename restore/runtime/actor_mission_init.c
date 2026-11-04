#include "actor_mission_init.h"
#include "actor_create.h"
#include "editor_validation.h"
#include "town_menu.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
extern void FUN_0040f8f0(uint32_t *destination,uint32_t bytes,uint8_t value);
extern uint32_t FUN_00562348(void);
/* Whole existing 45-instruction integer leaf, authenticated and audited:
   only low WORDs of kind,x,y are consumed; five cdecl slots, RET0. */
extern int32_t FUN_004b6ff0(uint16_t kind,int32_t mode,int32_t orientation,uint16_t x,uint16_t y);

__declspec(noinline) void restore_actor_reset_pool(void)
{
 uint32_t record;
 for(record=0xbcd5f8u;record<0xc8ad38u;record+=388u)
  FUN_0040f8f0((uint32_t *)(uintptr_t)record,388,0);
 U(0xbcd5f0u)=0;U(0xbcd5f4u)=0;
}
__declspec(noinline) void FUN_004b4760(void){restore_actor_reset_pool();}
__declspec(noinline) void restore_town_actor_links_00448a60(void *self)
{
 uint32_t p=(uint32_t)(uintptr_t)self;
 W(p+0x3au)=0;W(p+0x3cu)=0;W(p+0x3eu)=0;W(p+0x40u)=0;
 W(p+0x42u)=0;W(p+0x44u)=0;W(p+0x46u)=0;W(p+0x4au)=0;
}
__declspec(noinline) void restore_actor_reset_town_links(void)
{
 RestoreTownRecord *record=restore_town_next(RESTORE_TOWNS,0,1);
 while(record){
  restore_town_actor_links_00448a60(record);
  record=restore_town_next(RESTORE_TOWNS,(const int16_t *)record,1);
 }
 U(0xd0563cu)=0;U(0xd05638u)=0;U(0xd05634u)=0;
 U(0xd05630u)=0;U(0xd0562cu)=0;
}
__declspec(noinline) void FUN_004b8c90(void){restore_actor_reset_town_links();}
__declspec(noinline) void restore_actor_initialize_reserved(void)
{
 uint32_t actor,tag;
 FUN_004b4760();FUN_004b8c90();
 for(tag=1;tag<=3u;++tag){
  actor=(uint32_t)FUN_004b4800(1,36,-1,-1,0);
  B(0xbcd603u+actor*388u)=(uint8_t)tag;
 }
 /* Native fourth allocation has no tag store, including on failure. */
 (void)FUN_004b4800(1,36,-1,-1,0);
 for(tag=12;tag<=32u;++tag){
  actor=(uint32_t)FUN_004b4800(1,36,-1,-1,0);
  B(0xbcd603u+actor*388u)=(uint8_t)tag;
 }
 for(tag=50;tag<=65u;++tag){
  actor=(uint32_t)FUN_004b4800(1,36,-1,-1,0);
  B(0xbcd603u+actor*388u)=(uint8_t)tag;
 }
}
static void boundary(uint32_t site,uint32_t target,struct restore_actor_init_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
#define A(n) U(sp+(n)*4u)
 switch(target){
 case 0x4b4760u:FUN_004b4760();break;
 case 0x4b8c90u:FUN_004b8c90();break;
 case 0x4b4800u:
  f->r[0]=(uint32_t)FUN_004b4800((uint8_t)A(1),(int32_t)A(2),(int32_t)A(3),(int32_t)A(4),(uint8_t)A(5));break;
 case 0x4b6ff0u:
  f->r[0]=(uint32_t)FUN_004b6ff0((uint16_t)A(1),(int32_t)A(2),(int32_t)A(3),(uint16_t)A(4),(uint16_t)A(5));break;
 case 0x4ec410u:f->r[0]=FUN_004ec410(A(1),(char)A(2));break;
 case 0x562348u:f->r[0]=FUN_00562348();break;
 default:abort();
 }
 /* All six genuine native targets return with RET0. The model owns ADD ESP.
    ECX/EDX and flags from these calls are dead before any full-width use.
    Partial CX/DX writes after allocation feed WORD stores only, then reload. */
 f->r[4]=sp+4u;
#undef A
}
static void execute(void (*body)(struct restore_actor_init_frame *))
{
 uint32_t stack[64],i,entry;struct restore_actor_init_frame f;
 for(i=0;i<64;++i)stack[i]=SENTRY;
 for(i=0;i<8;++i)f.r[i]=0;
 entry=(uint32_t)(uintptr_t)(stack+32);
 f.r[4]=entry;f.flags=2;f.boundary=boundary;
 /* Own minima incl CALL return slots: BB090 -92, BB3F0 -72, BBC10 -44.
    Typed real callees use their own C stacks; all own RET paths balance. */
 body(&f);
 if(f.r[4]!=entry+4u||stack[32]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<8;++i)if(stack[i]!=SENTRY)abort();
 for(i=33;i<64;++i)if(stack[i]!=SENTRY)abort();
}
void FUN_004bb090(void){restore_actor_initialize_reserved();}
void FUN_004bb3f0(void){execute(restore_actor_init_C_004bb3f0);}
void FUN_004bbc10(void){execute(restore_actor_init_C_004bbc10);}
