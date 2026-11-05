#include "crt_format.h"

char *restore_crt_format_general(const double *value,char *buffer,int32_t precision,int32_t capitals)
{
 RestoreCrtDecimal *decimal;
 char *digits,*cursor;
 int32_t exponent;
 uint8_t rounded;
 RESTORE_CRT_DECIMAL=FUN_00568d88(restore_crt_binary64_load_spill(value));
 decimal=RESTORE_CRT_DECIMAL;
 RESTORE_CRT_EXPONENT=restore_crt_add32(decimal->exponent,-1);
 digits=buffer+(decimal->sign=='-');
 FUN_00568d11(digits,precision,decimal);
 decimal=RESTORE_CRT_DECIMAL;
 exponent=restore_crt_add32(decimal->exponent,-1);
 rounded=(uint8_t)(RESTORE_CRT_EXPONENT<exponent);
 RESTORE_CRT_ROUNDED=rounded;
 /* Native code reloads the descriptor exponent after the byte store. */
 exponent=restore_crt_add32(decimal->exponent,-1);
 RESTORE_CRT_EXPONENT=exponent;
 if(exponent< -4||exponent>=precision)return FUN_005644e0(value,buffer,precision,capitals);
 if(rounded!=0){
  cursor=digits;
  do {++cursor;}while(cursor[-1]!=0);
  cursor[-2]=0;
 }
 return FUN_00564507(value,buffer,precision);
}
