#include "monument_rating.h"
#include "campaign_score_numeric.h"
#include "monument_base_queries.h"
#include "monument_complex_queries.h"
#include "monument_progress.h"
#include "pyramid_summary.h"
#include "editor_brush_validation.h"
#include "tool_selector.h"
#include <stdlib.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define SELF(a) ((void *)(uintptr_t)(a))
extern int32_t FUN_004cde40(int32_t,int32_t);

/* Native read-only binary64 constants at 570128/56F690/56F520/56F5A0. */
static const uint64_t rating_scale=UINT64_C(0x401947ae147ae148);
static const uint64_t rating_half=UINT64_C(0x3fe0000000000000);
static const uint64_t rating_one=UINT64_C(0x3ff0000000000000);
static const uint64_t rating_zero=0;

static void zero80(uint8_t value[10])
{
 __asm {
  fld qword ptr rating_zero
  mov eax,value
  fstp tbyte ptr[eax]
 }
}
static void add_integer80(uint8_t value[10],int32_t integer)
{
 __asm {
  mov eax,value
  fld tbyte ptr[eax]
  fild integer
  faddp st(1),st(0)
  fstp tbyte ptr[eax]
 }
}
static void add_rounded64(uint8_t value[10],uint64_t *rounded,int32_t integer)
{
 /* Native 4F7C77 adds the PREVIOUS binary64 spill, stores binary64,
    and retains its extended result for the resource loop/final sqrt. */
 __asm {
  fild integer
  mov edx,rounded
  fadd qword ptr[edx]
  fst qword ptr[edx]
  mov eax,value
  fstp tbyte ptr[eax]
 }
}
static void finish80(uint8_t value[10],int incomplete)
{
 uint16_t status;
 __asm {
  mov eax,value
  fld tbyte ptr[eax]
  fsqrt
  fmul qword ptr rating_scale
  fadd qword ptr rating_half
 }
 if(incomplete){
  __asm {
   fsub qword ptr rating_one
   fld qword ptr rating_zero
   fcomp st(1)
   fnstsw status
  }
  if(!(status&0x4100u)){
   __asm {
    fstp st(0)
    fld qword ptr rating_zero
   }
  }
 }
 __asm {
  mov eax,value
  fstp tbyte ptr[eax]
 }
}

/* Existing whole 4EF450 has no calls, four saved registers and RET0.
   The native argument slot is scratch and may be overwritten by the query. */
static void query_pyramid(uint32_t building)
{
 uint32_t stack[32]={0},sp=(uint32_t)(uintptr_t)(stack+16);
 struct restore_pyramid_summary_frame f={{0},2,restore_pyramid_summary_dispatch};
 stack[17]=building;f.r[4]=sp;
 restore_pyramid_C_004ef450(&f);
 if(f.r[4]!=sp+4u)abort();
}

/* Whole 4F53D0..4F546B, 60 instructions. Return value is AL only. */
uint8_t FUN_004f53d0(uint32_t building)
{
 uint32_t record=building*264u,category;
 if(B(0x799a2du+record)>1u)return 0;
 query_pyramid(building);
 if(U(0xeaaf58u))return 0;
 category=B(0x799a2cu+record);
 if(FUN_004ecea0((int32_t)category)){
  int32_t delta=(int32_t)(U(0x5f5d08u+category*4u)+U(0x7999e4u+record)-U(0xbca2d8u));
  FUN_004ed360(delta%228,delta/228,3,0);
  if(U(0xeaae90u))return 0;
 }
 return 1;
}

/* Reviewed native maps at 4F7D40/68 and 4F7E00/28. In both loops the
   initial ECX=-1 iteration is skipped, so key0 addresses array DWORD1. */
static const uint8_t resource_active[30]={
 1,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,1,1,1,1,0,0,1,1,1,1,0,1,0,1
};
static const uint8_t resource_divisor[30]={
 32,16,16,16,16,16,16,32,16,16,16,16,16,16,16,16,16,32,16,32,16,16,16,32,32,32,16,16,16,32
};

/* Complete contiguous 4F78A0..4F7D37, 368 instructions. All five native
   indirect switches are closed here, including code missing from Ghidra's
   original 105-instruction function body. This function uses no C floats. */
__declspec(noinline) void restore_monument_rating_raw80(int32_t selected,uint8_t result[10])
{
 uint32_t slot,key;
 int incomplete=0;
 uint64_t rounded=0;
 zero80(result);
 if(FUN_004f4ff0()){
  if((uint8_t)selected){
   for(slot=0;slot<3u;++slot){
    int32_t selection=(int32_t)(int16_t)W(0x784de0u+slot*2u);
    if(selection)add_integer80(result,(int32_t)(int16_t)W(0x5d19d4u+(uint32_t)selection*16u));
   }
  }else{
   int32_t summary[28];
   for(slot=0;slot<3u;++slot){
    uint32_t offset=(U(0xeaac28u)*0x1276u+slot)*4u;
    uint32_t building=U(0xea5facu+offset),record,metadata;
    int32_t progress,kind,part;
    if(!building)continue;
    record=building*264u;metadata=0x5d19c8u+U(0xea5fa0u+offset)*16u;
    kind=(int32_t)(int16_t)W(0x7999e8u+record);
    switch(kind){
    case 0xb7:
     if(W(0x799a10u+record))continue;
     (void)FUN_004f1310(building,summary);
     if(!FUN_004f53d0(building))incomplete=1;
     progress=summary[13];break;
    case 0xcf:case 0xd0:
     progress=B(0x799a2du+record);if(progress<100)incomplete=1;break;
    case 0xd2:{
     int32_t count=(int32_t)(int16_t)W(0xea5baau+U(0xeaac28u)*0x49d8u);
     if(count<16)incomplete=1;
     progress=count*100/16;break;
    }
    case 0xd5:progress=FUN_00542ad0(building,0);goto check_progress;
    case 0xde:progress=FUN_004df040(building,0);goto check_progress;
    case 0xe1:progress=restore_monument_base_0040d940(SELF(0x6fd5d8u),building,0);goto check_progress;
    case 0xe3:progress=restore_monument_base_00417050(SELF(0x6fd368u),building,0);goto check_progress;
    case 0xe4:progress=restore_monument_base_004e6710(SELF(0x6de548u),building,0);goto check_progress;
    case 0xe5:progress=restore_monument_progress_00545740(SELF(0x6f9320u),building,0);goto check_progress;
    case 0xe6:progress=FUN_00401540(building,0);goto check_progress;
    case 0xea:progress=restore_monument_progress_00558be0(SELF(0x6f5648u),building,0);goto check_progress;
    case 0xeb:progress=restore_monument_progress_00555e80(SELF(0x6de9e0u),building,0);goto check_progress;
    case 0xec:progress=restore_monument_progress_0054f470(SELF(0x6e9188u),building,0);goto check_progress;
    default:continue;
    }
    goto accumulate;
check_progress:
    if(progress<100)incomplete=1;
accumulate:
    part=FUN_004cde40((int32_t)(int16_t)W(metadata+12u),progress);
    add_rounded64(result,&rounded,part);
   }
  }
  for(key=0;key<30u;++key){
   uint32_t address=0x784decu+key*4u,value;
   if(!resource_active[key]||!U(address))continue;
   if(!(uint8_t)selected)address+=0x90u;
   value=U(address);
   add_integer80(result,(int32_t)value/resource_divisor[key]);
  }
 }
 finish80(result,incomplete);
}

int32_t restore_monument_rating_int32(int32_t selected)
{
 uint8_t raw80[10];
 restore_monument_rating_raw80(selected,raw80);
 return(int32_t)(uint32_t)restore_campaign_score_trunc80(raw80);
}

__declspec(naked) void FUN_004f78a0(int32_t selected)
{
 (void)selected;
 __asm {
  sub esp,12
  mov eax,esp
  mov ecx,dword ptr[esp+16]
  push eax
  push ecx
  call restore_monument_rating_raw80
  add esp,8
  fld tbyte ptr[esp]
  add esp,12
  ret
 }
}

/* Whole nullary 46EF50, 19 instructions. Clamp belongs to native code;
   player indexing and both stores are DWORD operations. */
void FUN_0046ef50(void)
{
 int32_t value=restore_monument_rating_int32(0);
 uint32_t address=0xea5b44u+U(0xeaac28u)*0x49d8u;
 U(address)=(uint32_t)value;
 if(value<0)U(address)=0;
 if((int32_t)U(address)>100)U(address)=100;
 U(0xea5cacu+U(0xeaac28u)*0x49d8u)=0;
}
