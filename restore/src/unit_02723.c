#include "restore_types.h"

void FUN_00560199(void)

{
  int iVar1;
  int in_ECX;
  undefined4 local_8;
  
  if (*(int *)(in_ECX + 0x200) == 0) {
    for (local_8 = 0; local_8 < *(int *)(in_ECX + 0x1e4); local_8 = local_8 + 1) {
      iVar1 = _AIL_3D_sample_status_4(*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + local_8 * 4));
      if (iVar1 == 4) {
        _AIL_3D_update_position_8(*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + local_8 * 4),0);
      }
    }
  }
  return;
}
