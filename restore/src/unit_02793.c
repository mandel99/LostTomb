#include "crt_local_unwind.h"

int32_t restore_crt_abnormal_termination(void)
{
 volatile const RestoreCrtUnwindGuard *guard=
  (volatile const RestoreCrtUnwindGuard *)(uintptr_t)__readfsdword(0);
 uint32_t registration,parent_level;
 if(guard->handler!=(uint32_t)(uintptr_t)FUN_005617c8)return 0;
 registration=guard->registration;
 parent_level=*(volatile uint32_t *)(uintptr_t)(registration+12u);
 return guard->level==parent_level?1:0;
}
