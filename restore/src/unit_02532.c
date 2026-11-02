#include "restore_types.h"

void FUN_005404b0(undefined4 *param_1)

{
  int *in_ECX;
  
  param_1[1] = in_ECX[1];
  if ((undefined4 *)in_ECX[1] != (undefined4 *)0x0) {
    *(undefined4 *)in_ECX[1] = param_1;
  }
  *param_1 = 0;
  in_ECX[1] = (int)param_1;
  if (*in_ECX == 0) {
    *in_ECX = (int)param_1;
  }
  return;
}
