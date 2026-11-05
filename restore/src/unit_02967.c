#include "crt_significand96.h"

/* 56896E: the existing add helper supplies the true full-EAX carry.
   Signed division truncates toward zero; native CL masks the shift to5 bits.
   The loop keeps its word pointer and carry across each preceding word. */
uint32_t restore_crt96_increment_bit(uint32_t *words,int32_t bit)
{
 int32_t index=bit/32;
 uint32_t base=(uint32_t)(uintptr_t)words;
 uint32_t shift=(31u-(uint32_t)(bit%32))&31u;
 uint32_t address=base+(uint32_t)index*4u,carry;
 uint32_t *word=(uint32_t *)(uintptr_t)address;
 carry=FUN_00569e0c(*word,1u<<shift,word);
 --index;
 if(index>=0){
  address=base+(uint32_t)index*4u;
  do{
   if(carry==0)return carry;
   word=(uint32_t *)(uintptr_t)address;
   carry=FUN_00569e0c(*word,1u,word);
   --index;address-=4u;
  }while(index>=0);
 }
 return carry;
}

uint32_t FUN_0056896e(int32_t words,int32_t bit)
{return restore_crt96_increment_bit((uint32_t *)(uintptr_t)(uint32_t)words,bit);}
