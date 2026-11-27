#include "pharaoh/core/projectile_angle.h"
#include "pharaoh/core/x87_numeric.h"

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This inherited-precision angle owner requires MSVC Win32.
#endif

/* Exact binary64 data read from pinned Ghidra570130..57014F/56F5A0. */
static const uint64_t native_zero=0;
static const uint64_t native_angle_scale=UINT64_C(0x40145d051315a466);
static const uint64_t native_turn=UINT64_C(0x401924a8c154c986);
static const uint64_t native_angle_bias=UINT64_C(0x40160013a92a3055);
static const uint64_t native_half_turn=UINT64_C(0x400924a8c154c986);

/* Whole0051CAE0: current/old Unit02331 C reused with the complete C asin
 * child, including its real error/control paths. No restricted AngleAsin. */
uint8_t pharaoh_projectile_angle(int32_t x1,int32_t y1,int32_t x2,int32_t y2,
    const PharaohCrtMathEnvironment *environment)
{
 int32_t deltaX=(int32_t)((uint32_t)x2-(uint32_t)x1);
 int32_t deltaY=(int32_t)((uint32_t)y1-(uint32_t)y2);
 uint64_t vertical,radius;uint16_t status;PharaohFloat80 raw80;int64_t result;
 __asm { fild deltaX }
 __asm { fild deltaY }
 __asm { fst qword ptr vertical }
 __asm { fld st(0) }
 __asm { fmulp st(1),st(0) }
 __asm { fld st(1) }
 __asm { fmul st(0),st(2) }
 __asm { faddp st(1),st(0) }
 __asm { fsqrt }
 __asm { fst qword ptr radius }
 __asm { fcomp qword ptr native_zero }
 __asm { fnstsw status }
 if(status&0x4000u){__asm { fstp st(0) }return 0;}
 __asm { fcomp qword ptr native_zero }
 __asm { fld qword ptr vertical }
 __asm { fdiv qword ptr radius }
 __asm { fnstsw status }
 if(status&0x4100u){
  pharaoh_crt_asin_live(environment);
 __asm { fsubr qword ptr native_half_turn }
 }else pharaoh_crt_asin_live(environment);
 __asm { fsub qword ptr native_angle_bias }
 __asm { fcom qword ptr native_zero }
 __asm { fnstsw status }
 while(status&0x100u){
 __asm { fadd qword ptr native_turn }
 __asm { fcom qword ptr native_zero }
  __asm { fnstsw status }
 }
 __asm { fld qword ptr native_turn }
 __asm { fsub st(0),st(1) }
 __asm { fmul qword ptr native_angle_scale }
 __asm { lea eax,raw80 }
 __asm { fstp tbyte ptr[eax] }
 result=pharaoh_x87_truncate80(&raw80);
 __asm { fstp st(0) }
 return (uint8_t)result;
}
