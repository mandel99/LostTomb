#include <stdint.h>
#include "menu_full_input_numeric.h"

/*51CAE0's projectile callers provide signed WORD map coordinates and the
  game's double/extended x87 precision. Their asin ratio is finite in[-1,1].
  Lower that reachable562D00/562D1D contract using the native operation order,
  precision control, endpoint literal and masked inexact-result spill. */
static __declspec(noinline) void angle_asin(void)
{
 uint16_t saved,work,status;double rounded;uint32_t pi2=0x6050aau;
 __asm { fnstcw saved }
 work=(uint16_t)((saved&0x300u)|0x7fu);
 if(saved!=0x27fu){__asm { fldcw work }}
 __asm { fld st(0) }
 __asm { fabs }
 __asm { fld1 }
 __asm { fcompp }
 __asm { fnstsw status }
 if(status&0x4000u){
  __asm { ftst }
  __asm { fnstsw status }
  __asm { fstp st(0) }
  __asm { mov eax,pi2 }
  __asm { fld tbyte ptr[eax] }
  if(status&0x100u){__asm { fchs }}
 }else{
  __asm { fld1 }
  __asm { fadd st(0),st(1) }
  __asm { fld1 }
  __asm { fsub st(0),st(2) }
  __asm { fmulp st(1),st(0) }
  __asm { fsqrt }
  __asm { fpatan }
 }
 __asm { fnstsw status }
 if(saved!=0x27fu&&(saved&0x20u)&&(status&0x20u)&&
    !*(volatile uint32_t *)(uintptr_t)0x12663c0u){
  /* The original masked precision handler stores/reloads a double result. */
  __asm { fstp qword ptr rounded }
  __asm { fld qword ptr rounded }
 }
 if(saved!=0x27fu){__asm { fldcw saved }}
}
uint8_t FUN_0051cae0(int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 int32_t delta_x=(int32_t)((uint32_t)x2-(uint32_t)x1);
 int32_t delta_y=(int32_t)((uint32_t)y1-(uint32_t)y2);
 double vertical,radius;uint16_t status;uint8_t raw80[10];uint64_t result;
 uint32_t zero=0x56f5a0u,pi=0x570148u,angle_offset=0x570140u,tau=0x570138u,scale=0x570130u;
 __asm { fild delta_x }
 __asm { fild delta_y }
 __asm { fst qword ptr vertical }
 __asm { fld st(0) }
 __asm { fmulp st(1),st(0) }
 __asm { fld st(1) }
 __asm { fmul st(0),st(2) }
 __asm { faddp st(1),st(0) }
 __asm { fsqrt }
 __asm { fst qword ptr radius }
 __asm { mov eax,zero }
 __asm { fcomp qword ptr[eax] }
 __asm { fnstsw status }
 if(status&0x4000u){__asm { fstp st(0) }return 0;}
 __asm { mov eax,zero }
 __asm { fcomp qword ptr[eax] }
 __asm { fld qword ptr vertical }
 __asm { fdiv qword ptr radius }
 __asm { fnstsw status }
 if(status&0x4100u){
  angle_asin();
  __asm { mov eax,pi }
  __asm { fsubr qword ptr[eax] }
 }else angle_asin();
 __asm { mov eax,angle_offset }
 __asm { fsub qword ptr[eax] }
 __asm { mov eax,zero }
 __asm { fcom qword ptr[eax] }
 __asm { fnstsw status }
 while(status&0x100u){
  __asm { mov eax,tau }
  __asm { fadd qword ptr[eax] }
  __asm { mov eax,zero }
  __asm { fcom qword ptr[eax] }
  __asm { fnstsw status }
 }
 __asm { mov eax,tau }
 __asm { fld qword ptr[eax] }
 __asm { fsub st(0),st(1) }
 __asm { mov eax,scale }
 __asm { fmul qword ptr[eax] }
 __asm { lea eax,raw80 }
 __asm { fstp tbyte ptr[eax] }
 result=restore_menu_input_trunc80(raw80);
 __asm { fstp st(0) }
 return (uint8_t)result;
}
