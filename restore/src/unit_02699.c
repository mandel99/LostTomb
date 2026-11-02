#include "restore_types.h"
#include "sound_initialization.h"

int FUN_0055edd0(uintptr_t in_ECX,const int32_t *param_1,int param_2)

{
  undefined4 uVar1;
  int local_c;
  int local_8;
  
  local_8 = -1;
  if (*(int *)(in_ECX + 0x200) == 0) {
    if (*(int *)(*(int *)(in_ECX + 8) + param_1[1] * 8) == 0) {
      local_8 = -1;
    }
    else if (**(char **)(*(int *)(in_ECX + 8) + param_1[1] * 8) == '\0') {
      local_8 = -1;
    }
    else {
      for (local_c = 0; local_c < *(int *)(in_ECX + 0x198); local_c = local_c + 1) {
        if (*(int *)(*(int *)(in_ECX + 0x1ac) + 8 + local_c * 0xc) == 0) {
          local_8 = local_c;
          break;
        }
      }
      if (local_8 == -1) {
        local_8 = -1;
      }
      else {
        if (*(int *)(*(int *)(in_ECX + 0x1ac) + 4 + local_8 * 0xc) != 0) {
          _AIL_mem_free_lock_4(*(undefined4 *)(*(int *)(in_ECX + 0x1ac) + 4 + local_8 * 0xc));
          FUN_0055e5d2((RestoreSound *)(uintptr_t)in_ECX,param_2);
        }
        *(undefined4 *)(*(int *)(in_ECX + 0x1a8) + param_2 * 0x28) = *param_1;
        *(undefined4 *)(*(int *)(in_ECX + 0x1a8) + 4 + param_2 * 0x28) = param_1[1];
        *(undefined4 *)(*(int *)(in_ECX + 0x1a8) + 8 + param_2 * 0x28) = param_1[2];
        *(undefined4 *)(*(int *)(in_ECX + 0x1a8) + 0xc + param_2 * 0x28) = param_1[3];
        *(undefined4 *)(*(int *)(in_ECX + 0x1a8) + 0x10 + param_2 * 0x28) = param_1[4];
        uVar1 = _AIL_file_read_8(*(undefined4 *)(*(int *)(in_ECX + 8) + param_1[1] * 8),0);
        *(undefined4 *)(*(int *)(in_ECX + 0x1ac) + 4 + local_8 * 0xc) = uVar1;
        if (*(int *)(*(int *)(in_ECX + 0x1ac) + 4 + local_8 * 0xc) == 0) {
          local_8 = -1;
        }
        else {
          *(undefined4 *)(*(int *)(in_ECX + 0x1ac) + local_8 * 0xc) = param_1[1];
          _AIL_init_sample_4(*(undefined4 *)(*(int *)(in_ECX + 0x19c) + param_2 * 4));
          _AIL_set_sample_file_12
                    (*(undefined4 *)(*(int *)(in_ECX + 0x19c) + param_2 * 4),
                     *(undefined4 *)(*(int *)(in_ECX + 0x1ac) + 4 + local_8 * 0xc),0);
        }
      }
    }
  }
  else {
    local_8 = 1;
  }
  return local_8;
}
