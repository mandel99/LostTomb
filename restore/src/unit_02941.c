#include "crt_startup_arguments.h"
#include "crt_runtime_tail.h"
void *FUN_00561b68(size_t bytes);
void FUN_00561bdc(void *allocation);
size_t FUN_00563380(const char *text);
uint32_t *FUN_00566750(void *destination,const void *source);

void restore_crt_initialize_environment(void)
{
 uint32_t count=0;
 char *entry;
 char **slot;
 if(RESTORE_CRT_MB_INITIALIZED==0)FUN_00569bf5();
 entry=RESTORE_CRT_ENVIRONMENT_BLOCK;
 while(*entry!=0){
  if(*entry!='=')++count;
  entry+=(uint32_t)FUN_00563380(entry)+1u;
 }
 slot=(char **)FUN_00561b68(count*4u+4u);
 RESTORE_CRT_ENVIRONMENT_VECTOR=slot;
 if(slot==0)(void)FUN_005630e0(9);
 /* Allocation can change the global; the native second pass reloads it. */
 entry=RESTORE_CRT_ENVIRONMENT_BLOCK;
 while(*entry!=0){
  uint32_t bytes=(uint32_t)FUN_00563380(entry)+1u;
  if(*entry!='='){
   *slot=(char *)FUN_00561b68(bytes);
   if(*slot==0)(void)FUN_005630e0(9);
   (void)FUN_00566750(*slot,entry);
   ++slot;
  }
  entry+=bytes;
 }
 FUN_00561bdc(RESTORE_CRT_ENVIRONMENT_BLOCK);
 RESTORE_CRT_ENVIRONMENT_BLOCK=0;
 *slot=0;
 RESTORE_CRT_ENVIRONMENT_READY=1;
}
void FUN_00567994(void){restore_crt_initialize_environment();}
