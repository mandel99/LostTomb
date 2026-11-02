#include "restore_types.h"

void FUN_00560113(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int in_ECX;
  
  if (*(int *)(in_ECX + 0x200) == 0) {
    _AIL_set_3D_position_16
              (*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + param_1 * 4),param_2,param_3,param_4);
  }
  return;
}
