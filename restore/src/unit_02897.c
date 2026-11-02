#include "restore_types.h"
extern void FUN_00565307();

void FUN_0056536d(char *param_1,int param_2,undefined4 param_3,int *param_4)

{
  char cVar1;
  
  do {
    if (param_2 < 1) {
      return;
    }
    param_2 = param_2 + -1;
    cVar1 = *param_1;
    param_1 = param_1 + 1;
    FUN_00565307((int)cVar1,param_3,param_4);
  } while (*param_4 != -1);
  return;
}
