#include "restore_types.h"

int FUN_0055faf7(void)

{
  int iVar1;
  int in_ECX;
  int local_70;
  
  if ((*(int *)(in_ECX + 0x164) != 5) && (*(int *)(in_ECX + 0x200) == 0)) {
    for (local_70 = 0; local_70 < *(int *)(in_ECX + 0x1e4); local_70 = local_70 + 1) {
      iVar1 = _AIL_3D_sample_status_4(*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + local_70 * 4));
      if (iVar1 != 4) {
        return local_70;
      }
    }
  }
  return -1;
}
