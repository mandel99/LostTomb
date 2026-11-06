#include "crt_multibyte.h"
#include "crt_nls_mapping.h"
#include "crt_eh3_frames.h"

static uint32_t run_multibyte_uppercase(volatile uint8_t *output,
 const volatile uint32_t *arguments,uint32_t parent_frame,uint32_t recovered)
{
 uint32_t character=arguments[0];
 volatile uint8_t *argument_bytes=(volatile uint8_t *)arguments;
 (void)parent_frame;(void)recovered;
 if(character<=0xffu){
  if((RESTORE_CRT_MB_FLAGS[character+1]&0x20u)==0x20u)
   return RESTORE_CRT_MB_CASE_MAP[character];
 }else{
  /* Native mutates the high two bytes of its incoming stack argument. */
  argument_bytes[3]=(uint8_t)character;
  argument_bytes[2]=(uint8_t)(character>>8);
  if((RESTORE_CRT_MB_FLAGS[((character>>8)&0xffu)+1]&4u)!=0 &&
     FUN_0056947e(RESTORE_CRT_MB_LOCALE,0x200,(LPCSTR)(argument_bytes+2),2,
      (void *)output,2,RESTORE_CRT_CODEPAGE,1)!=0)
   return (uint32_t)output[0]*256u+output[1];
 }
 return character;
}
/* Native PUSH ECX seeds the two output bytes. A one-byte mapping leaves
   the second byte live; capture the real incoming register before C calls. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) uint32_t restore_crt_uppercase_multibyte(uint32_t character)
{
 __asm push ebp
 __asm mov ebp,esp
 __asm push ecx
 __asm push ebx
 __asm push esi
 __asm push edi
 RESTORE_EH3_BODY_CALL(4,run_multibyte_uppercase,0)
 RESTORE_EH3_PARENT_RETURN
}
__declspec(naked) uint32_t FUN_0056b363(uint32_t character)
{__asm jmp restore_crt_uppercase_multibyte}
#pragma warning(pop)
