#include "restore_types.h"

void FUN_0055fbd8(void)

{
  int iVar1;
  int in_ECX;
  undefined4 local_8;
  
  for (local_8 = 0; local_8 < *(int *)(in_ECX + 0x1e4); local_8 = local_8 + 1) {
    *(undefined4 *)(*(int *)(in_ECX + 0x1f8) + 8 + local_8 * 0xc) = 0;
  }
  for (local_8 = 0; local_8 < *(int *)(in_ECX + 0x1e4); local_8 = local_8 + 1) {
    iVar1 = _AIL_3D_sample_status_4(*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + local_8 * 4));
    if (iVar1 == 4) {
      *(int *)(*(int *)(in_ECX + 0x1f8) + 8 +
              *(int *)(*(int *)(in_ECX + 500) + 0x18 + local_8 * 0x28) * 0xc) =
           *(int *)(*(int *)(in_ECX + 0x1f8) + 8 +
                   *(int *)(*(int *)(in_ECX + 500) + 0x18 + local_8 * 0x28) * 0xc) + 1;
    }
  }
  return;
}
