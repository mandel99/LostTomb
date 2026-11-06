#include "crt_multibyte.h"

uint32_t restore_crt_test_multibyte_character(uint8_t character,uint32_t scalar_mask,uint8_t multibyte_mask)
{
 if((RESTORE_CRT_MB_FLAGS[(uint32_t)character+1]&multibyte_mask)!=0)return 1;
 if(scalar_mask!=0){
  /* Native WORD table stride is two bytes, independently of DAT storage units. */
  uint16_t attributes=((volatile const uint16_t *)0x00604c62u)[character];
  if((attributes&scalar_mask)!=0)return 1;
 }
 return 0;
}
uint32_t FUN_00569800(uint8_t character,uint32_t scalar_mask,uint8_t multibyte_mask)
{return restore_crt_test_multibyte_character(character,scalar_mask,multibyte_mask);}
