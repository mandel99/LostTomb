#include "restore_types.h"
extern undefined4 FUN_0055eb99();

undefined4 FUN_0055ec0f(void)

{
  int iVar1;
  int in_ECX;
  undefined4 local_8;
  
  if (*(int *)(in_ECX + 0x200) == 0) {
    for (local_8 = 0; local_8 < *(int *)(in_ECX + 0x198); local_8 = local_8 + 1) {
      iVar1 = _AIL_sample_loop_count_4(*(undefined4 *)(*(int *)(in_ECX + 0x19c) + local_8 * 4));
      if (iVar1 == 0) {
        FUN_0055eb99(local_8);
      }
    }
  }
  return 1;
}
