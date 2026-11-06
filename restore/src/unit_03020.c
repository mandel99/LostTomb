#include "crt_environment.h"

int32_t restore_crt_find_environment_name(const uint8_t *name,uint32_t length)
{
 char **slot=RESTORE_CRT_ENVIRONMENT_VECTOR;
 char *entry=*slot;
 uint32_t difference;
 int32_t index;
 uint32_t found=0;
 while(entry!=0){
  if(FUN_0056a857(name,(const uint8_t *)entry,length)==0){
   uint8_t delimiter=(uint8_t)(*slot)[length];
   if(delimiter=='=' || delimiter==0){found=1;break;}
  }
  entry=slot[1];
  ++slot;
 }
 /* Native SAR of a wrapping DWORD difference uses a freshly loaded base,
    even when comparison callbacks have replaced the global vector pointer. */
 difference=(uint32_t)(uintptr_t)slot-(uint32_t)(uintptr_t)RESTORE_CRT_ENVIRONMENT_VECTOR;
 index=(int32_t)difference>>2;
 return found?index:(int32_t)(0u-(uint32_t)index);
}
int32_t FUN_0056b045(const uint8_t *name,uint32_t length)
{return restore_crt_find_environment_name(name,length);}
