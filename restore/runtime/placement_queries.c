#include "placement_queries.h"
#include "monument_images.h"
#include "monument_entrance.h"
#include "monument_complex_positions.h"
#include "monument_complex_images.h"
#include <stdio.h>
#include <stdlib.h>
#include "monument_progress.h"
#include "sprite_libraries.h"
extern uint32_t FUN_004ea930(void);
#define MI_U8(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define MI_U16(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MI_U32(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Named C providers retain the native member ABI at this frame boundary. */

static __declspec(noreturn) void query_failure(uint32_t target)
{fprintf(stderr,"Unreconstructed placement query %08lx\n",(unsigned long)target);abort();}
/* Genuine547BA0: +38 returns a fixed piece count for these four native
   classes; +24 is the already reconstructed5477E0 worker query. */
uint32_t restore_placement_activity_00547ba0(uint32_t self,int32_t building)
{
 uint32_t original=(uint32_t)building,root=original-(uint32_t)(int32_t)(int16_t)MI_U16(0x7999eau+original*264u),eax,index,count,target;
 eax=MI_U32(0x799a70u+root*264u);if((int32_t)eax>0)return eax&0xffffff00u;
 target=MI_U32(MI_U32(self)+0x38u);
 switch(target){
 case 0x545f60u:count=9;break;case 0x559460u:count=10;break;
 case 0x5567b0u:count=21;break;case 0x54fed0u:count=31;break;
 default:query_failure(target);
 }
 eax=count;
 for(index=0;(int32_t)index<(int32_t)count;++index){
  target=MI_U32(MI_U32(self)+0x24u);if(target!=0x5477e0u)query_failure(target);
  eax=original+index;
  /* Native5477E0 modifies AL only. Its canonical AL facade additionally
     preserves the genuine invalid-worker slot-clear side effect. */
  eax=(eax&0xffffff00u)|restore_monument_worker_active(original+index);
  if((uint8_t)eax)return eax&0xffffff00u;
 }
 return (eax&0xffffff00u)|1u;
}
void restore_placement_queries_dispatch(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 restore_menu_input_frame_call outer=f->boundary;uint32_t sp=f->r[4];(void)site;
 f->boundary=restore_placement_queries_dispatch;
 switch(target){
 case 0x401800u:f->r[0]=restore_monument_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),f);f->r[4]=sp+12u;break;
 case 0x40de80u:f->r[0]=restore_monument_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),f);f->r[4]=sp+12u;break;
 case 0x417310u:f->r[0]=restore_monument_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),f);f->r[4]=sp+12u;break;
 case 0x545760u:f->r[0]=restore_complex_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x558c00u:f->r[0]=restore_complex_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x555ea0u:f->r[0]=restore_complex_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x54f490u:f->r[0]=restore_complex_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x4e6a70u:f->r[0]=restore_monument_image_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),f);f->r[4]=sp+12u;break;
 case 0x401af0u:f->r[0]=restore_monument_entrance_query(target,f->r[1],MI_U32(f->r[4]+4u),MI_U32(f->r[4]+8u),MI_U32(f->r[4]+12u));f->r[4]+=16u;break;
 case 0x40eb90u:f->r[0]=restore_monument_entrance_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),MI_U32(sp+12u));f->r[4]=sp+16u;break;
 case 0x417490u:f->r[0]=restore_monument_entrance_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),MI_U32(sp+12u));f->r[4]=sp+16u;break;
 case 0x545a30u:f->r[0]=restore_complex_position_query(target,MI_U32(sp+4u),MI_U32(sp+8u),MI_U32(sp+12u));f->r[4]=sp+16u;break;
 case 0x558f00u:f->r[0]=restore_complex_position_query(target,MI_U32(sp+4u),MI_U32(sp+8u),MI_U32(sp+12u));f->r[4]=sp+16u;break;
 case 0x556230u:f->r[0]=restore_complex_position_query(target,MI_U32(sp+4u),MI_U32(sp+8u),MI_U32(sp+12u));f->r[4]=sp+16u;break;
 case 0x54f8a0u:f->r[0]=restore_complex_position_query(target,MI_U32(sp+4u),MI_U32(sp+8u),MI_U32(sp+12u));f->r[4]=sp+16u;break;
 case 0x4e6ae0u:f->r[0]=restore_monument_entrance_query(target,f->r[1],MI_U32(sp+4u),MI_U32(sp+8u),MI_U32(sp+12u));f->r[4]=sp+16u;break;
 case 0x547ba0u:f->r[0]=restore_placement_activity_00547ba0(f->r[1],(int32_t)MI_U32(sp+4u));f->r[4]=sp+8u;break;
 /* Exact no-argument tail adapter545FB0 -> genuine23-instruction4EA930. */
 case 0x545fb0u:f->r[0]=FUN_004ea930();f->r[4]=sp+4u;break;
 /* Native RET8: manager receiver is genuine incoming ECX, not a guessed
    singleton. These41 callers overwrite EAX before consuming its high bits. */
 case 0x53dd90u:
  f->r[0]=(f->r[0]&0xffffff00u)|FUN_0053dd90((RestoreSpriteLibraries *)(uintptr_t)f->r[1],(const char *)(uintptr_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x545740u:f->r[0]=(uint32_t)restore_monument_progress_00545740((void *)(uintptr_t)f->r[1],MI_U32(sp+4u),(int32_t *)(uintptr_t)MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x558be0u:f->r[0]=(uint32_t)restore_monument_progress_00558be0((void *)(uintptr_t)f->r[1],MI_U32(sp+4u),(int32_t *)(uintptr_t)MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x555e80u:f->r[0]=(uint32_t)restore_monument_progress_00555e80((void *)(uintptr_t)f->r[1],MI_U32(sp+4u),(int32_t *)(uintptr_t)MI_U32(sp+8u));f->r[4]=sp+12u;break;
 case 0x54f470u:f->r[0]=(uint32_t)restore_monument_progress_0054f470((void *)(uintptr_t)f->r[1],MI_U32(sp+4u),(int32_t *)(uintptr_t)MI_U32(sp+8u));f->r[4]=sp+12u;break;
 default:query_failure(target);
 }
 f->boundary=outer;
}
static uint32_t query_run(uint32_t target,uint32_t self,int32_t building,uint32_t first,uint32_t second,uint32_t arguments)
{
 uint32_t stack[192],i,entry;struct restore_menu_input_frame f={{0},2,restore_placement_queries_dispatch};
 typedef char pointer_is32[sizeof(void*)==4?1:-1];(void)sizeof(pointer_is32);
 for(i=0;i<192;++i)stack[i]=0x8e47b123u;
 entry=(uint32_t)(uintptr_t)(stack+160);f.r[1]=self;f.r[4]=entry;
 stack[161]=(uint32_t)building;stack[162]=first;if(arguments==3)stack[163]=second;
 restore_placement_queries_dispatch(0,target,&f);
 if(f.r[4]!=entry+4u+arguments*4u||stack[160]!=0x8e47b123u||f.r[3]||f.r[5]||f.r[6]||f.r[7])query_failure(target);
 for(i=0;i<16;++i)if(stack[i]!=0x8e47b123u)query_failure(target);
 for(i=161u+arguments;i<192;++i)if(stack[i]!=0x8e47b123u)query_failure(target);
 return f.r[0];
}
uint32_t restore_placement_image_query(uint32_t self,int32_t building,uint32_t out_flags)
{return query_run(MI_U32(MI_U32(self)+0x1cu),self,building,out_flags,0,2);}
uint32_t restore_placement_position_query(uint32_t self,int32_t building,uint32_t out_x,uint32_t out_y)
{return query_run(MI_U32(MI_U32(self)+0x20u),self,building,out_x,out_y,3);}
