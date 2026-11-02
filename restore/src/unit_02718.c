#include "restore_types.h"
extern void FUN_0055fa1c();

int FUN_0055fca4(undefined4 *param_1,int param_2)

{
  undefined4 uVar1;
  int in_ECX;
  int local_10;
  int local_c;
  
  local_c = -1;
  if (*(int *)(*(int *)(in_ECX + 8) + param_1[1] * 8) == 0) {
    local_c = -1;
  }
  else if (**(char **)(*(int *)(in_ECX + 8) + param_1[1] * 8) == '\0') {
    local_c = -1;
  }
  else if (*(int *)(in_ECX + 0x200) == 0) {
    for (local_10 = 0; local_10 < *(int *)(in_ECX + 0x1e4); local_10 = local_10 + 1) {
      if (*(int *)(*(int *)(in_ECX + 0x1f8) + 8 + local_10 * 0xc) == 0) {
        local_c = local_10;
        break;
      }
    }
    if (local_c == -1) {
      local_c = -1;
    }
    else {
      if (*(int *)(*(int *)(in_ECX + 0x1f8) + 4 + local_c * 0xc) != 0) {
        _AIL_mem_free_lock_4(*(undefined4 *)(*(int *)(in_ECX + 0x1f8) + 4 + local_c * 0xc));
        FUN_0055fa1c(param_2);
      }
      *(undefined4 *)(*(int *)(in_ECX + 500) + param_2 * 0x28) = *param_1;
      *(undefined4 *)(*(int *)(in_ECX + 500) + 4 + param_2 * 0x28) = param_1[1];
      *(undefined4 *)(*(int *)(in_ECX + 500) + 8 + param_2 * 0x28) = param_1[2];
      *(undefined4 *)(*(int *)(in_ECX + 500) + 0x10 + param_2 * 0x28) = param_1[4];
      *(undefined4 *)(*(int *)(in_ECX + 500) + 0x1c + param_2 * 0x28) = param_1[7];
      *(undefined4 *)(*(int *)(in_ECX + 500) + 0x20 + param_2 * 0x28) = param_1[8];
      *(undefined4 *)(*(int *)(in_ECX + 500) + 0x24 + param_2 * 0x28) = param_1[9];
      uVar1 = _AIL_file_read_8(*(undefined4 *)(*(int *)(in_ECX + 8) + param_1[1] * 8),0);
      *(undefined4 *)(*(int *)(in_ECX + 0x1f8) + 4 + local_c * 0xc) = uVar1;
      if (*(int *)(*(int *)(in_ECX + 0x1f8) + 4 + local_c * 0xc) == 0) {
        local_c = -1;
      }
      else {
        *(undefined4 *)(*(int *)(in_ECX + 0x1f8) + local_c * 0xc) = param_1[1];
        _AIL_set_3D_sample_file_8
                  (*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + param_2 * 4),
                   *(undefined4 *)(*(int *)(in_ECX + 0x1f8) + 4 + local_c * 0xc));
      }
    }
  }
  else {
    local_c = -1;
  }
  return local_c;
}
