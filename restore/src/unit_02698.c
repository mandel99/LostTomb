#include "restore_types.h"
#include "sound_initialization.h"

void FUN_0055ecf3(uintptr_t in_ECX)

{
  int iVar1;
  undefined4 local_8;
  
  if (*(int *)(in_ECX + 0x200) == 0) {
    for (local_8 = 0; local_8 < *(int *)(in_ECX + 0x198); local_8 = local_8 + 1) {
      *(undefined4 *)(*(int *)(in_ECX + 0x1ac) + 8 + local_8 * 0xc) = 0;
    }
    for (local_8 = 0; local_8 < *(int *)(in_ECX + 0x198); local_8 = local_8 + 1) {
      iVar1 = _AIL_sample_status_4(*(undefined4 *)(*(int *)(in_ECX + 0x19c) + local_8 * 4));
      if (iVar1 == 4) {
        *(int *)(*(int *)(in_ECX + 0x1ac) + 8 +
                *(int *)(*(int *)(in_ECX + 0x1a8) + 0x18 + local_8 * 0x28) * 0xc) =
             *(int *)(*(int *)(in_ECX + 0x1ac) + 8 +
                     *(int *)(*(int *)(in_ECX + 0x1a8) + 0x18 + local_8 * 0x28) * 0xc) + 1;
      }
    }
  }
  return;
}
