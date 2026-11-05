#include "crt_format.h"

char *restore_crt_general_fixed(const double *value,char *buffer,int32_t precision)
{
 char *result;
 RESTORE_CRT_REUSE=1;
 result=FUN_00564367(value,buffer,precision);
 RESTORE_CRT_REUSE=0;
 return result;
}
