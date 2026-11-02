#include "restore_types.h"
#include "sound_initialization.h"
extern void * FUN_00561a1d();
extern void FUN_00561a2b();

undefined4 FUN_0055e14f(int param_1)

{
  undefined4 uVar1;
  void *pvVar2;
  int in_ECX;
  undefined4 local_8;
  
  if (param_1 < 1) {
    uVar1 = 0;
  }
  else {
    if (*(int *)(in_ECX + 0x1f0) != 0) {
      FUN_0055ec7c((RestoreSound *)(uintptr_t)in_ECX);
      for (local_8 = 0; local_8 < *(int *)(in_ECX + 0x1e4); local_8 = local_8 + 1) {
        _AIL_release_3D_sample_handle_4(*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + local_8 * 4));
        if (*(int *)(*(int *)(in_ECX + 0x1f8) + 4 + local_8 * 0xc) != 0) {
          _AIL_mem_free_lock_4(*(undefined4 *)(*(int *)(in_ECX + 0x1f8) + 4 + local_8 * 0xc));
          *(undefined4 *)(*(int *)(in_ECX + 0x1f8) + 4 + local_8 * 0xc) = 0;
        }
      }
    }
    if (*(int *)(in_ECX + 0x1f0) != 0) {
      FUN_00561a2b(*(undefined4 *)(in_ECX + 0x1f0));
      *(undefined4 *)(in_ECX + 0x1f0) = 0;
    }
    if (*(int *)(in_ECX + 500) != 0) {
      FUN_00561a2b(*(undefined4 *)(in_ECX + 500));
      *(undefined4 *)(in_ECX + 500) = 0;
    }
    if (*(int *)(in_ECX + 0x1f8) != 0) {
      FUN_00561a2b(*(undefined4 *)(in_ECX + 0x1f8));
      *(undefined4 *)(in_ECX + 0x1f8) = 0;
    }
    pvVar2 = FUN_00561a1d(param_1 << 2);
    *(void **)(in_ECX + 0x1f0) = pvVar2;
    if (*(int *)(in_ECX + 0x1f0) == 0) {
      uVar1 = 0;
    }
    else {
      pvVar2 = FUN_00561a1d(param_1 * 0x28);
      *(void **)(in_ECX + 500) = pvVar2;
      if (*(int *)(in_ECX + 500) == 0) {
        uVar1 = 0;
      }
      else {
        pvVar2 = FUN_00561a1d(param_1 * 0xc);
        *(void **)(in_ECX + 0x1f8) = pvVar2;
        if (*(int *)(in_ECX + 0x1f8) == 0) {
          uVar1 = 0;
        }
        else {
          *(int *)(in_ECX + 0x1e4) = param_1;
          for (local_8 = 0; local_8 < *(int *)(in_ECX + 0x1e4); local_8 = local_8 + 1) {
            uVar1 = _AIL_allocate_3D_sample_handle_4(*(undefined4 *)(in_ECX + 0x1e8));
            *(undefined4 *)(*(int *)(in_ECX + 0x1f0) + local_8 * 4) = uVar1;
            if (*(int *)(*(int *)(in_ECX + 0x1f0) + local_8 * 4) == 0) break;
          }
          uVar1 = 1;
        }
      }
    }
  }
  return uVar1;
}
