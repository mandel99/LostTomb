#include "restore_types.h"
extern undefined4 FUN_004f3370();
extern int FUN_004f3690();

undefined4 FUN_004f3850(undefined4 param_1)

{
  char cVar1;
  undefined4 local_4;
  
  cVar1 = FUN_004f3370(param_1,&local_4,&param_1,0);
  if (cVar1 != '\0') {
    FUN_004f3690(local_4,param_1);
    return 1;
  }
  return 0;
}
