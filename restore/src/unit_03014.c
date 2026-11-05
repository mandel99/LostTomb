#include "crt_format.h"

/* Unaligned DWORD phases in the native twelve-byte product accumulator. */
static uint32_t product_dword(const RestoreCrt96 *p,uint32_t offset)
{
 return *(const uint32_t *)(const void *)(p->byte+offset);
}
static void product_store(RestoreCrt96 *p,uint32_t offset,uint32_t value)
{
 *(uint32_t *)(void *)(p->byte+offset)=value;
}
void restore_crt_multiply_raw96(RestoreCrt96 *left,const RestoreCrt96 *right)
{
 RestoreCrt96 product={{0,0,0}};
 uint16_t left_exponent=(uint16_t)(left->half[5]&0x7fffu);
 uint16_t right_exponent=(uint16_t)(right->half[5]&0x7fffu);
 uint16_t sign=(uint16_t)((left->half[5]^right->half[5])&0x8000u);
 uint32_t exponent=(uint32_t)left_exponent+(uint32_t)right_exponent;
 uint32_t outer,inner,offset,a,b,sum;
 int32_t shifts;
 uint32_t sticky=0;
 if(left_exponent>=0x7fffu||right_exponent>=0x7fffu||(uint16_t)exponent>0xbffdu)goto overflow;
 if((uint16_t)exponent<=0x3fbfu)goto zero;
 if(left_exponent==0){
  ++exponent;
  if((left->word[2]&0x7fffffffu)==0&&left->word[1]==0&&left->word[0]==0){
   /* This native early exit writes only the sign/exponent WORD. */
   left->half[5]=0;
   return;
  }
 }
 if(right_exponent==0){
  ++exponent;
  if((right->word[2]&0x7fffffffu)==0&&right->word[1]==0&&right->word[0]==0)goto zero;
 }
 /* Five partial diagonals, each accumulating a DWORD and a WORD carry.
    The lower discarded diagonals are absent in the original algorithm. */
 for(outer=0;outer<5u;++outer){
  offset=outer*2u;
  for(inner=0;inner<5u-outer;++inner){
   a=product_dword(&product,offset);
   b=(uint32_t)left->half[outer+inner]*(uint32_t)right->half[4u-inner];
   sum=a+b;
   product_store(&product,offset,sum);
   if(sum<a||sum<b)product.half[outer+2u]=(uint16_t)(product.half[outer+2u]+1u);
  }
 }
 exponent+=0xc002u;
 if((int16_t)exponent>0){
  while((product.byte[11]&0x80u)==0&&(int16_t)exponent>0){
   FUN_00569e8b(product.word);
   exponent+=0xffffu;
  }
 }
 if((int16_t)exponent<=0){
  exponent+=0xffffu;
  if((int16_t)exponent<0){
   shifts=-(int32_t)(int16_t)exponent;
   exponent+=(uint32_t)shifts;
   do{
    if((product.byte[0]&1u)!=0)++sticky;
    FUN_00569eb9(product.word);
    --shifts;
   }while(shifts!=0);
   if(sticky!=0)product.byte[0]|=1u;
  }
 }
 if(product.half[0]>0x8000u||(product.word[0]&0x1ffffu)==0x18000u){
  a=product_dword(&product,2);
  if(a!=0xffffffffu)product_store(&product,2,a+1u);
  else{
   product_store(&product,2,0);
   a=product_dword(&product,6);
   if(a!=0xffffffffu)product_store(&product,6,a+1u);
   else{
    product_store(&product,6,0);
    if(product.half[5]!=0xffffu)++product.half[5];
    else{++exponent;product.half[5]=0x8000u;}
   }
  }
 }
 if((uint16_t)exponent>=0x7fffu)goto overflow;
 left->half[0]=product.half[1];
 product_store(left,2,product.word[1]);
 product_store(left,6,product.word[2]);
 left->half[5]=(uint16_t)((uint16_t)exponent|sign);
 return;
zero:
 left->word[2]=0;left->word[1]=0;left->word[0]=0;
 return;
overflow:
 left->word[1]=0;left->word[0]=0;
 left->word[2]=0x7fff8000u+(sign!=0?0x80000000u:0u);
}
