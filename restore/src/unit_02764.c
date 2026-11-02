#include "restore_types.h"

int FUN_00560fa3(undefined1 *param_1,int param_2)

{
  int iVar1;
  int *in_ECX;
  int local_8;
  
  local_8 = 0;
  do {
    if (param_2 == 0) {
      return local_8;
    }
    param_2 = param_2 + -1;
    if ((in_ECX[2] == 0) && ((undefined1 *)in_ECX[7] < (undefined1 *)in_ECX[8])) {
      *(undefined1 *)in_ECX[7] = *param_1;
      in_ECX[7] = in_ECX[7] + 1;
    }
    else {
      iVar1 = (**(code **)(*in_ECX + 0x1c))(*param_1);
      if (iVar1 == -1) {
        return local_8;
      }
    }
    param_1 = param_1 + 1;
    local_8 = local_8 + 1;
  } while( true );
}
