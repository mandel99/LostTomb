#include "restore_types.h"

uint FUN_00560b2a(uint param_1)

{
  int *in_ECX;
  
  if ((undefined1 *)in_ECX[7] < (undefined1 *)in_ECX[8]) {
    *(undefined1 *)in_ECX[7] = (undefined1)param_1;
    in_ECX[7] = in_ECX[7] + 1;
    param_1 = param_1 & 0xff;
  }
  else {
    param_1 = (**(code **)(*in_ECX + 0x1c))(param_1);
  }
  return param_1;
}
