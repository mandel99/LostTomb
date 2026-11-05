#include "crt_format.h"

char *restore_crt_format_fixed(const double *value,char *buffer,int32_t precision)
{
 RestoreCrtDecimal *decimal;
 char *point;
 int32_t exponent,zeros;
 if(RESTORE_CRT_REUSE!=0){
  exponent=RESTORE_CRT_EXPONENT;
  decimal=RESTORE_CRT_DECIMAL;
  if(exponent==precision){
   point=restore_crt_offset(buffer,restore_crt_add32(exponent,decimal->sign=='-'));
   point[0]='0';point[1]=0;
  }
 }else{
  decimal=FUN_00568d88(restore_crt_binary64_load_spill(value));
  FUN_00568d11(buffer+(decimal->sign=='-'),restore_crt_add32(decimal->exponent,precision),decimal);
 }
 point=buffer;
 if(decimal->sign=='-'){*point='-';++point;}
 exponent=decimal->exponent;
 if(exponent<=0){FUN_0056457b(point,1);*point='0';++point;}
 else point=restore_crt_offset(point,exponent);
 if(precision>0){
  FUN_0056457b(point,1);
  *point=RESTORE_CRT_POINT;
  exponent=decimal->exponent;
  ++point;
  if(exponent<0){
   zeros=restore_crt_neg32(exponent);
   if(RESTORE_CRT_REUSE!=0||precision>=zeros)precision=zeros;
   FUN_0056457b(point,precision);
   FUN_00563240(point,'0',(size_t)(uint32_t)precision);
  }
 }
 return buffer;
}
