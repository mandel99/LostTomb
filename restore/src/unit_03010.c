#include "crt_format.h"

uint32_t restore_crt_raw80_decimal(uint32_t low,uint32_t high,uint32_t sign_exponent,int32_t requested,uint32_t flags,RestoreCrtDecimalDigits *output)
{
 RestoreCrt96 tenth={{0xccccccccu,0xccccccccu,0x3ffbccccu}};
 RestoreCrt96 fraction,previous;
 uint32_t exponent=sign_exponent&0x7fffu;
 uint32_t estimate_bits;
 int16_t decimal_exponent;
 int32_t count,shift,remaining;
 char *next,*last,*first;
 int8_t length;
 uint8_t rounding_digit;
 const char *special;
 output->sign=(int8_t)((sign_exponent&0x8000u)?'-':' ');
 if(exponent==0&&high==0&&low==0)goto zero;
 if(exponent==0x7fffu){
  output->exponent=1;
  if((high==0x80000000u&&low==0)||(high&0x40000000u)!=0){
   if((sign_exponent&0x8000u)!=0&&high==0xc0000000u&&low==0)special=(const char *)(uintptr_t)0x570c14u;
   else if(high==0x80000000u&&low==0)special=(const char *)(uintptr_t)0x570c0cu;
   else special=(const char *)(uintptr_t)0x570c04u;
   FUN_00566750(output->digits,special);
   output->length=(int8_t)((special==(const char *)(uintptr_t)0x570c04u)?6:5);
  }else{
   FUN_00566750(output->digits,(const void *)(uintptr_t)0x570c1cu);
   output->length=6;
  }
  return 0;
 }
 /* Native approximate log10 estimate, with DWORD arithmetic and SAR. */
 estimate_bits=((exponent>>8)+(high>>24)*2u)*0x4du+exponent*0x4d10u+0xecbced0cu;
 decimal_exponent=(int16_t)((int32_t)estimate_bits>>16);
 fraction.half[0]=0;
 fraction.half[1]=(uint16_t)low;
 fraction.half[2]=(uint16_t)(low>>16);
 fraction.half[3]=(uint16_t)high;
 fraction.half[4]=(uint16_t)(high>>16);
 fraction.half[5]=(uint16_t)exponent;
 FUN_0056ab24(&fraction,-(int32_t)decimal_exponent,1);
 if(fraction.half[5]>=0x3fffu){
  decimal_exponent=(int16_t)((uint16_t)decimal_exponent+1u);
  FUN_0056a904(&fraction,&tenth);
 }
 output->exponent=decimal_exponent;
 count=requested;
 if((flags&1u)!=0){
  count=restore_crt_add32(requested,(int32_t)decimal_exponent);
  if(count<=0)goto zero;
 }
 if(count>21)count=21;
 shift=(int32_t)fraction.half[5]-0x3ffe;
 fraction.half[5]=0;
 for(remaining=8;remaining!=0;--remaining)FUN_00569e8b(fraction.word);
 if(shift<0){
  remaining=(int32_t)((0u-(uint32_t)shift)&0xffu);
  while(remaining>0){FUN_00569eb9(fraction.word);--remaining;}
 }
 next=output->digits;
 remaining=restore_crt_add32(count,1);
 if(remaining>0){
  do{
   previous=fraction;
   FUN_00569e8b(fraction.word);
   FUN_00569e8b(fraction.word);
   FUN_00569e2d(fraction.word,previous.word);
   FUN_00569e8b(fraction.word);
   rounding_digit=fraction.byte[11];
   fraction.byte[11]=0;
   *next=(char)(rounding_digit+(uint8_t)'0');
   ++next;--remaining;
  }while(remaining!=0);
 }
 rounding_digit=(uint8_t)*restore_crt_offset(next,-1);
 last=restore_crt_offset(next,-2);
 first=output->digits;
 if((int8_t)rounding_digit>='5'){
  while((uintptr_t)last>=(uintptr_t)first&&*last=='9'){*last='0';--last;}
  if((uintptr_t)last<(uintptr_t)first){++last;output->exponent=(int16_t)((uint16_t)output->exponent+1u);}
  *last=(char)((uint8_t)*last+1u);
 }else{
  while((uintptr_t)last>=(uintptr_t)first&&*last=='0')--last;
  if((uintptr_t)last<(uintptr_t)first)goto zero;
 }
 /* Native AL subtraction yields the logical signed byte digit length. */
 length=(int8_t)((uint8_t)(uintptr_t)last-(uint8_t)(uintptr_t)output-3u);
 output->length=length;
 *((char *)output+4+(int32_t)length)=0;
 return 1;
zero:
 output->exponent=0;
 output->sign=' ';
 output->length=1;
 output->digits[0]='0';
 output->digits[1]=0;
 return 1;
}
