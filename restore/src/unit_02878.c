#include "crt_format.h"

char *restore_crt_general_scientific(const double *value,char *buffer,int32_t precision,int32_t capitals)
{
 char *result;
 RESTORE_CRT_REUSE=1;
 result=FUN_00564263(value,buffer,precision,capitals);
 RESTORE_CRT_REUSE=0;
 return result;
}
