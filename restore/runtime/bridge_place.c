#include "bridge_place.h"
#include "world_offset_model.h"
#include "road_place.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
uint32_t restore_bridge_00440540(uint32_t native_args[3],uint32_t incoming_ecx)
{
 uint32_t building=U((uintptr_t)native_args),record=building*264u,image_b=incoming_ecx,direction;
 int32_t linked,x,y,x2,y2;
 direction=U(0x799aacu+record);linked=H(0x799a12u+record);
 FUN_004405c0((int32_t)direction,(int32_t *)native_args,(int32_t *)&image_b);
 x=(int32_t)U((uintptr_t)(native_args+1));y=(int32_t)U((uintptr_t)(native_args+2));
 if(direction==0){x2=x;y2=(int32_t)((uint32_t)y+1u);}
 else{x2=(int32_t)((uint32_t)x+1u);y2=y;}
 FUN_0047bf50((uint16_t)building,x,y,1,1,U((uintptr_t)native_args),8);
 FUN_0047bf50((uint16_t)linked,x2,y2,1,1,image_b,8);
 return image_b;
}
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Physical bridge entry requires the MSVC x86 ABI
#endif
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) void FUN_00440540(int32_t building,int32_t x,int32_t y)
{
 __asm {
  push ecx
  lea eax,[esp+8]
  push eax
  call restore_bridge_00440540
  add esp,8
  mov ecx,eax
  ret
 }
}
#pragma warning(pop)
