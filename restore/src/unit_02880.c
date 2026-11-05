#include "crt_format.h"

char *restore_crt_format_float(const double *value,char *buffer,int32_t format,int32_t precision,int32_t capitals)
{
 if(format=='e'||format=='E')return FUN_00564263(value,buffer,precision,capitals);
 if(format=='f')return FUN_00564367(value,buffer,precision);
 return FUN_00564445(value,buffer,precision,capitals);
}
