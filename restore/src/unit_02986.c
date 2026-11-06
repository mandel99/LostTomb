#include "crt_multibyte.h"
#include "crt_eh3_frames.h"

static void prepare_wide_character(void *frame,const volatile uint32_t *arguments,uint32_t saved_sp)
{(void)frame;(void)arguments;(void)saved_sp;}
static int32_t run_wide_character(void *frame,const volatile uint32_t *arguments,
 uint32_t parent_frame,uint32_t recovered)
{
 uint32_t destination=arguments[0];
 volatile uint32_t *used_default=(volatile uint32_t *)arguments;
 (void)frame;(void)parent_frame;(void)recovered;
 if(destination==0)return 0;
 if(RESTORE_CRT_CTYPE_HANDLE==0){
  uint16_t character=*(volatile const uint16_t *)(arguments+1);
  if(character<=0xffu){*(uint8_t *)(uintptr_t)destination=(uint8_t)character;return 1;}
 }else{
  int32_t count;
  /* Native BOOL output aliases the first incoming argument word. */
  *used_default=0;
  count=WideCharToMultiByte(RESTORE_CRT_LOCALE_CODEPAGE,0x220,
   (LPCWSTR)(arguments+1),1,(LPSTR)(uintptr_t)destination,
   (int32_t)RESTORE_CRT_MAX_MULTIBYTE_BYTES,0,(LPBOOL)used_default);
  if(count!=0 && *used_default==0)return count;
 }
 RESTORE_CRT_ERRNO=0x2a;
 return -1;
}
RESTORE_EH3_PARENT(restore_crt_wide_character_to_multibyte,int32_t,
 (LPSTR destination,uint16_t character),0,prepare_wide_character,run_wide_character)
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) int32_t FUN_005691e5(LPSTR destination,uint16_t character)
{__asm jmp restore_crt_wide_character_to_multibyte}
#pragma warning(pop)
