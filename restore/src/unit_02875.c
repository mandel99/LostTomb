#include "crt_format.h"

char *restore_crt_format_scientific(const double *value,char *buffer,int32_t precision,int32_t capitals)
{
 RestoreCrtDecimal *decimal;
 char *number,*exponent;
 int32_t magnitude;
 if(RESTORE_CRT_REUSE!=0){
  decimal=RESTORE_CRT_DECIMAL;
  FUN_0056457b(buffer+(decimal->sign=='-'),precision>0);
 }else{
  decimal=FUN_00568d88(restore_crt_binary64_load_spill(value));
  FUN_00568d11(buffer+(decimal->sign=='-')+(precision>0),restore_crt_add32(precision,1),decimal);
 }
 number=buffer;
 if(decimal->sign=='-'){*number='-';++number;}
 if(precision>0){number[0]=number[1];++number;*number=RESTORE_CRT_POINT;}
 /* The reuse byte is reloaded here, after conversion/insertion. */
 exponent=restore_crt_offset(number,restore_crt_add32(precision,RESTORE_CRT_REUSE==0));
 exponent=(char *)FUN_00566750(exponent,(const void *)(uintptr_t)0x5706e8u);
 if(capitals!=0)*exponent='E';
 if(decimal->digits[0]!='0'){
  magnitude=restore_crt_add32(decimal->exponent,-1);
  if(magnitude<0){magnitude=restore_crt_neg32(magnitude);exponent[1]='-';}
  if(magnitude>=100){exponent[2]=(char)((uint8_t)exponent[2]+(uint8_t)(magnitude/100));magnitude%=100;}
  if(magnitude>=10){exponent[3]=(char)((uint8_t)exponent[3]+(uint8_t)(magnitude/10));magnitude%=10;}
  exponent[4]=(char)((uint8_t)exponent[4]+(uint8_t)magnitude);
 }
 return buffer;
}
