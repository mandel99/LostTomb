#include "restore_types.h"

void FUN_00560150(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  int in_ECX;
  
  if (*(int *)(in_ECX + 0x200) == 0) {
    _AIL_set_3D_orientation_28
              (*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + param_1 * 4),param_2,param_3,param_4,
               param_5,param_6,param_7);
  }
  return;
}
