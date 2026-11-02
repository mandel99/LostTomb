#include "editor_map_reset.h"
#include "city_prepare.h"
#include "camera_bounds.h"
#include "lookup_sentinel.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "terrain_redraw.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f8f0(void *,uint32_t,uint8_t);
extern void FUN_00517a20(void);

/* Whole 444B10, seven instructions. The count is a DWORD after the fill. */
void FUN_00444b10(void)
{
 FUN_0040f8f0((void *)0x77af00u,0x2260u,0);
 U(0x77aefcu)=0;
}

/* Whole 46FDA0, 21 instructions. Native height and the row endpoints are
   signed DWORDs. REP STOSD fills exactly right-left cells, without a clamp. */
void FUN_0046fda0(void)
{
 int32_t height=(int32_t)U(0xbca2d4u);
 uint32_t row=0;
 if(height<=0)return;
 do{
  uint32_t left=U(0xbc9f24u+row*4u),right=U(0xbc9b94u+row*4u);
  if((int32_t)left<(int32_t)right){
   uint32_t address=0xb7482cu+U(0xbc9804u+row*4u)*4u,count=right-left;
   do{U(address)=6;address+=4u;}while(--count);
  }
  ++row;
 }while((int32_t)row<height);
}

/* Whole 4DE430..4DE5FF, 92 instructions, cdecl1/RET0. These constant
   address lists preserve the native WORD/DWORD reset order. */
void FUN_004de430(int32_t size_index)
{
 static const uint32_t word_slots[]={
  0x784c60u,0x784c62u,0x784c64u,0x784c66u,0x784c68u,
  0x784c6au,0x784c8cu,0x784c8eu,0x784c90u,0x784c92u
 };
 static const uint32_t dword_slots[]={
  0x784b74u,0x784b84u,0x784b78u,0x784b88u,0x784b7cu,0x784b8cu,
  0x784b80u,0x784cc0u,0x784b90u,0x784cc4u,0x784cd0u,0x784cc8u,
  0x784cd4u,0x784cccu,0x784cd8u,0x784dc4u,0x784cdcu,0x784dc8u,
  0x784b6cu,0x784dd0u,0x784dccu,0x784b70u,0x784dd4u,0x784b64u,
  0x784dd8u,0x784b68u
 };
 uint32_t table=0x5e2a64u+(uint32_t)size_index*16u,a,b,c,i;
 a=U(table);b=U(table+4u);U(0x784904u)=a;
 a=U(table+8u);U(0x784908u)=b;
 b=U(table+12u);U(0x78490cu)=a;U(0x784910u)=b;
 for(i=0;i<sizeof(word_slots)/sizeof(word_slots[0]);++i)W(word_slots[i])=0xffffu;
 for(i=0x784be0u;i<0x784c00u;i+=2u){W(i-32u)=0xffffu;W(i)=0xffffu;}
 for(i=0;i<sizeof(dword_slots)/sizeof(dword_slots[0]);++i)U(dword_slots[i])=UINT32_MAX;
 a=U(0x784904u);b=U(0x784908u);c=U(0x78490cu);
 U(0xbca2d0u)=a;a=U(0x784910u);
 U(0xbca2d4u)=b;U(0xbca2d8u)=c;U(0xbca2dcu)=a;
 FUN_0046f7b0();FUN_0046fa40();FUN_0046fda0();
 /* Native also pushes a second slot of 1 for 430530. The complete typed
    165-instruction provider only consumes the orientation argument. */
 FUN_00430530(U(0xe94e04u));
 a=U(0xbca2d4u);b=U(0xbca2d0u);
 FUN_004767d0(0,0,(int32_t)(b-1u),(int32_t)(a-1u),0,0);
 FUN_004ddf90();FUN_004de080();FUN_00517a20();
 FUN_00517370();FUN_00517660();FUN_005178e0();
 U(0xbca2bcu)=109;U(0xbca2c0u)=218;U(0xbca2e0u)=0;
}
