#include "crt_multibyte.h"
#include "crt_eh3_frames.h"
uint32_t *FUN_005685e0(uint32_t *text,char character);
static void prepare_multibyte_find(void *frame,const volatile uint32_t *arguments,uint32_t saved_sp)
{(void)frame;(void)arguments;(void)saved_sp;}
static uint8_t *run_multibyte_find(void *frame,const volatile uint32_t *arguments,
 uint32_t parent_frame,uint32_t recovered)
{
 uint8_t *text=(uint8_t *)(uintptr_t)arguments[0];
 uint32_t character=arguments[1];
 (void)frame;(void)parent_frame;(void)recovered;
 if(RESTORE_CRT_MB_ENABLED==0)return (uint8_t *)FUN_005685e0((uint32_t *)text,(char)character);
 for(;;){
  uint32_t first=*text;
  if(first==0)return character==0?text:0;
  if((RESTORE_CRT_MB_FLAGS[first+1]&4u)!=0){
   uint32_t second=text[1];
   if(second==0)return 0;
   if(character==first*256u+second)return text;
   text+=2;
  }else{
   if(character==first)return text;
   ++text;
  }
 }
}
/* The native caller words are read-only here. A separate C body prevents a
   narrow strchr tail call from rewriting their original high argument bytes. */
RESTORE_EH3_PARENT(restore_crt_find_multibyte_character,uint8_t *,
 (uint8_t *text,uint32_t character),0,prepare_multibyte_find,run_multibyte_find)
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) uint8_t *FUN_0056b104(uint8_t *text,uint32_t character)
{__asm jmp restore_crt_find_multibyte_character}
#pragma warning(pop)
