#include "campaign_score_numeric.h"
#define SCORE_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SCORE_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))

__declspec(noinline) uint64_t restore_campaign_score_trunc80(const uint8_t raw80[10])
{
 uint16_t original,truncate;uint64_t result;
 __asm {
  fnstcw original
  mov ax,original
  or ax,0c00h
  mov truncate,ax
  fldcw truncate
  mov eax,raw80
  fld tbyte ptr[eax]
  fistp qword ptr result
  fldcw original
 }
 return result;
}
int32_t FUN_00401540(uint32_t building,int32_t *output)
{
 uint32_t root=building-(uint32_t)(int32_t)(int16_t)SCORE_W(0x7999eau+building*264u);
 int32_t stage=(int32_t)SCORE_U(0x799a2cu+root*264u),result;
 if(stage==56)result=100;
 else{
  uint8_t raw80[10];
  __asm {
   fild stage
   mov eax,056f478h
   fmul qword ptr[eax]
   mov eax,056f470h
   fmul qword ptr[eax]
   lea eax,raw80
   fstp tbyte ptr[eax]
  }
  result=(int32_t)(uint32_t)restore_campaign_score_trunc80(raw80);
 }
 if(output){output[0]=stage;output[3]=result;}
 return result;
}
/* Exact 41C4FD..41C579 arithmetic: no binary32 or binary64 intermediate spill. */
__declspec(noinline) void restore_campaign_score_raw80(uint32_t scene_offset,uint32_t bonus,uint32_t months,uint8_t raw80[10])
{
 uint32_t sum,square;
 __asm {
  mov esi,scene_offset
  mov eax,months
  mov edx,dword ptr[esi+0ea5b48h]
  mov ebx,dword ptr[esi+0ea5b40h]
  mov edi,dword ptr[esi+0ea5b3ch]
  add edx,ebx
  fild bonus
  mov ecx,eax
  imul ecx,eax
  add edx,edi
  mov sum,edx
  fild sum
  fild dword ptr[esi+0ea1894h]
  mov square,ecx
  mov eax,056f540h
  fmul qword ptr[eax]
  fmulp st(1),st(0)
  fild square
  mov eax,056f538h
  fmul qword ptr[eax]
  fdivr st(0),st(2)
  fxch st(2)
  mov eax,056f530h
  fmul qword ptr[eax]
  fmulp st(2),st(0)
  fxch st(1)
  faddp st(1),st(0)
  fild dword ptr[esi+0ea187ch]
  fild months
  mov eax,056f528h
  fmul qword ptr[eax]
  fdivp st(1),st(0)
  faddp st(1),st(0)
  fild dword ptr[esi+0ea6018h]
  mov eax,056f520h
  fadd qword ptr[eax]
  mov eax,056f518h
  fmul qword ptr[eax]
  fmulp st(1),st(0)
  mov eax,raw80
  fstp tbyte ptr[eax]
 }
}
int32_t restore_campaign_score_numeric(uint32_t scene_offset,uint32_t bonus,uint32_t months)
{
 uint8_t raw80[10];uint32_t value;
 restore_campaign_score_raw80(scene_offset,bonus,months,raw80);
 value=(uint32_t)restore_campaign_score_trunc80(raw80);
 return(int32_t)(value-SCORE_U(scene_offset+0xea5d10u));
}
