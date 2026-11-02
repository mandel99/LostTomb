#include "pyramid_widget_draw.h"
#include "widget_position.h"
#include "sprite_render.h"
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Exact x87 primitives require MSVC Win32.
#endif
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* 41F8B0, six instructions, DWORD signed divide by two toward zero. */
uint32_t FUN_0041f8b0(void)
{return (uint32_t)((int32_t)U(0x704dc0u)/2)&1u;}

/* Whole 41F8C0, 23 instructions, cdecl1/RET0. REP MOVSD advances forward. */
void FUN_0041f8c0(int32_t orientation)
{
 uint32_t source,i;
 if(orientation==0||orientation==4){source=0x596f30u;U(0x704dc0u)=0;}
 else{source=0x597d10u;U(0x704dc0u)=2;}
 for(i=0;i<0x378u;++i)U(0x596150u+i*4u)=U(source+i*4u);
}

/* Exactly the native x87 operations, including the float stores. Constants
   authenticate as immutable PE float32 100,0.5,0 at 56F6DC/56F6D8/56F480.
   No CW changes, SSE substitutions, or extra live x87 values across draw. */
static void fp_initialize(int32_t denominator,int32_t progress,uint32_t *step,uint32_t *limit)
{
 uint32_t result_step,result_limit,hundred=0x42c80000u,half=0x3f000000u;
 __asm {
  fild dword ptr denominator
  fdivr dword ptr hundred
  fmul dword ptr half
  fstp dword ptr result_step
  fild dword ptr progress
  fstp dword ptr result_limit
 }
 *step=result_step;*limit=result_limit;
}
static uint32_t fp_add(uint32_t value,uint32_t step)
{
 uint32_t result;
 __asm {
  fld dword ptr value
  fadd dword ptr step
  fstp dword ptr result
 }
 return result;
}
static int fp_less(uint32_t value,uint32_t limit)
{
 uint16_t status;
 __asm {
  fld dword ptr value
  fcomp dword ptr limit
  fnstsw status
 }
 return (status&0x100u)!=0;
}

/* Entire 156-instruction 4E5E90..4E606A. Native cdecl3/RET0; EAX is
   incidental and dead at the 437342 caller. Native recycles all three
   outgoing argument slots; caller discards them at 437347 (ADD ESP,12). */
void FUN_004e5e90(int32_t building,int32_t x,int32_t y)
{
 uint32_t record=(uint32_t)building*264u,progress=B(0x799a2du+record);
 uint32_t level=0,kind,image,spacing,step,limit,accumulator=0,index;
 uint32_t base_x=(uint32_t)x,base_y=(uint32_t)y;
 if(progress>=100u)return;
 if(progress>=66u)level=(100u-progress)*3u;
 else if(progress>=33u)level=progress*3u-99u;
 if(level>=100u)level=100u;
 kind=W(0x7999e8u+record);
 if(kind==207u){base_x+=90u;base_y+=18u;spacing=1;}
 else{base_x+=152u;base_y+=14u;spacing=6;}
 image=U(0xf56d74u);
 fp_initialize(kind==207u?2:4,(int32_t)level,&step,&limit);
 if(fp_less(0,limit)){
  index=0;
  do{
   uint32_t draw_x=base_x+((index&1u)?spacing:0xffffffc4u-spacing);
   uint32_t draw_y=base_y-spacing/2u-(index/2u+1u)*60u;
   (void)restore_sprite_draw(RESTORE_SPRITE_LIBRARIES,(int32_t)image,(int32_t)draw_x,(int32_t)draw_y);
   accumulator=fp_add(accumulator,step);++index;
  }while(fp_less(accumulator,limit));
 }
 index=0;
 for(;;){
  uint32_t table,self,out_image;
  int32_t out_x,out_y;
  /* Native 4E5FEF reloads kind before EVERY iteration and termination test. */
  kind=W(0x7999e8u+record);
  if(index>=(kind==207u?10u:18u))break;
  table=kind==207u?0x586b08u:0x58fe48u;
  self=U(table+index*16u+8u);
  restore_widget_position_0041f910(self,&out_x,&out_y,&out_image);
  if(out_image){
   (void)restore_sprite_draw(RESTORE_SPRITE_LIBRARIES,(int32_t)out_image,
     (int32_t)(base_x+(uint32_t)out_x),(int32_t)(base_y+(uint32_t)out_y));
  }
  ++index;
 }
}
