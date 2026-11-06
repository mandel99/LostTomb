#include "crt_environment.h"
size_t FUN_00563380(const char *text);

char *restore_crt_get_environment_variable(const uint8_t *name)
{
 uint32_t ready=RESTORE_CRT_ENVIRONMENT_READY;
 char **slot=RESTORE_CRT_ENVIRONMENT_VECTOR;
 uint32_t length;
 if(ready==0)return 0;
 if(slot==0){
  if(RESTORE_CRT_WIDE_ENVIRONMENT_VECTOR==0 || FUN_0056a896()!=0)return 0;
  slot=RESTORE_CRT_ENVIRONMENT_VECTOR;
  if(slot==0)return 0;
 }
 if(name==0)return 0;
 length=(uint32_t)FUN_00563380((const char *)name);
 while(*slot!=0){
  uint32_t entry_length=(uint32_t)FUN_00563380(*slot);
  if(entry_length>length && (*slot)[length]=='=' &&
     FUN_0056a857((const uint8_t *)*slot,name,length)==0)
   return *slot+length+1u;
  ++slot;
 }
 return 0;
}
/* Existing native callers expose full pointer bits through an int32 ABI. */
int32_t FUN_005696a2(uint8_t *name)
{return (int32_t)(uint32_t)(uintptr_t)restore_crt_get_environment_variable(name);}
