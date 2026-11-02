#include "restore_types.h"
#include "sound_initialization.h"
extern void FUN_00561a2b();

undefined4 FUN_0055e92b(void)

{
  int in_ECX;
  undefined4 local_8;
  
  if (*(int *)(in_ECX + 0x200) == 0) {
    for (local_8 = 0; local_8 < *(int *)(in_ECX + 0x198); local_8 = local_8 + 1) {
      FUN_0055e5d2((RestoreSound *)(uintptr_t)in_ECX,local_8);
      _AIL_release_sample_handle_4(*(undefined4 *)(*(int *)(in_ECX + 0x19c) + local_8 * 4));
      if (*(int *)(*(int *)(in_ECX + 0x1ac) + 4 + local_8 * 0xc) != 0) {
        _AIL_mem_free_lock_4(*(undefined4 *)(*(int *)(in_ECX + 0x1ac) + 4 + local_8 * 0xc));
        *(undefined4 *)(*(int *)(in_ECX + 0x1ac) + 4 + local_8 * 0xc) = 0;
      }
    }
    for (local_8 = 0; local_8 < 3; local_8 = local_8 + 1) {
      if (*(int *)(in_ECX + 0x1d8 + local_8 * 4) != 0) {
        _AIL_close_stream_4(*(undefined4 *)(in_ECX + 0x1d8 + local_8 * 4));
        *(undefined4 *)(in_ECX + 0x1d8 + local_8 * 4) = 0;
      }
    }
    if (*(int *)(in_ECX + 0x164) != 5) {
      for (local_8 = 0; local_8 < *(int *)(in_ECX + 0x1e4); local_8 = local_8 + 1) {
        _AIL_release_3D_sample_handle_4(*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + local_8 * 4));
        if (*(int *)(*(int *)(in_ECX + 0x1f8) + 4 + local_8 * 0xc) != 0) {
          _AIL_mem_free_lock_4(*(undefined4 *)(*(int *)(in_ECX + 0x1f8) + 4 + local_8 * 0xc));
          *(undefined4 *)(*(int *)(in_ECX + 0x1f8) + 4 + local_8 * 0xc) = 0;
        }
      }
      _AIL_close_3D_provider_4(*(undefined4 *)(in_ECX + 0x1e8));
    }
    _AIL_shutdown_0();
    if (*(int *)(in_ECX + 0x1ac) != 0) {
      FUN_00561a2b(*(undefined4 *)(in_ECX + 0x1ac));
      *(undefined4 *)(in_ECX + 0x1ac) = 0;
    }
    if (*(int *)(in_ECX + 0x1a8) != 0) {
      FUN_00561a2b(*(undefined4 *)(in_ECX + 0x1a8));
      *(undefined4 *)(in_ECX + 0x1a8) = 0;
    }
    if (*(int *)(in_ECX + 0x1a0) != 0) {
      FUN_00561a2b(*(undefined4 *)(in_ECX + 0x1a0));
      *(undefined4 *)(in_ECX + 0x1a0) = 0;
    }
    if (*(int *)(in_ECX + 0x19c) != 0) {
      FUN_00561a2b(*(undefined4 *)(in_ECX + 0x19c));
      *(undefined4 *)(in_ECX + 0x19c) = 0;
    }
  }
  return 1;
}
