#include "crt_multibyte.h"
#include "crt_nls_mapping.h"

int32_t restore_crt_collate_multibyte_case_insensitive(const uint8_t *first,
 const uint8_t *second,uint32_t bytes)
{
 int32_t comparison;
 if(bytes==0)return 0;
 comparison=FUN_0056ac16(RESTORE_CRT_MB_LOCALE,1,(LPCSTR)first,(int32_t)bytes,
  (LPCSTR)second,(int32_t)bytes,RESTORE_CRT_CODEPAGE);
 if(comparison==0)return INT32_MAX;
 return (int32_t)((uint32_t)comparison-2u);
}
int32_t FUN_0056a857(const uint8_t *first,const uint8_t *second,uint32_t bytes)
{return restore_crt_collate_multibyte_case_insensitive(first,second,bytes);}
