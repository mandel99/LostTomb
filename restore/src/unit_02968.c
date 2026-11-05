#include "crt_significand96.h"

/* 5689C4: preserve the original guard/tail predicate and cached word address.
   Return the actual carry from56896E, then keep the native mask and clear
   following words. Cdecl callers provide the native forward direction. */
uint32_t restore_crt96_round_and_clear_tail(uint32_t *words,int32_t bit)
{
 int32_t index=bit/32,next;
 uint32_t base=(uint32_t)(uintptr_t)words;
 uint32_t shift=(31u-(uint32_t)(bit%32))&31u,carry=0;
 uint32_t *word=(uint32_t *)(uintptr_t)(base+(uint32_t)index*4u);
 if((*word&(1u<<shift))!=0&&
    FUN_00568925((int32_t)base,(int32_t)((uint32_t)bit+1u))==0){
  carry=restore_crt96_increment_bit(words,(int32_t)((uint32_t)bit-1u));
 }
 *word&=0xffffffffu<<shift;
 next=index+1;
 if(next<3){
  uint32_t address=base+(uint32_t)next*4u;
  uint32_t count=3u-(uint32_t)next;
  do{*(uint32_t *)(uintptr_t)address=0;address+=4u;--count;}while(count);
 }
 return carry;
}

uint32_t FUN_005689c4(int32_t words,int32_t bit)
{return restore_crt96_round_and_clear_tail((uint32_t *)(uintptr_t)(uint32_t)words,bit);}
