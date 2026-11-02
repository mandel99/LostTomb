#include "restore_types.h"
extern int FUN_0055faf7();
extern int FUN_0055fb6f();
extern void FUN_0055fbd8();
extern int FUN_0055fca4();

int FUN_0055ff20(int *param_1)

{
  char cVar1;
  int iVar2;
  int in_ECX;
  char local_14;
  int local_10;
  char local_8;
  
  if (*(int *)(in_ECX + 0x200) == 0) {
    if ((param_1[1] < 0) || (*(int *)(in_ECX + 4) + -1 < param_1[1])) {
      iVar2 = -1;
    }
    else {
      for (local_10 = 0; local_10 < *(int *)(in_ECX + 0x1e4); local_10 = local_10 + 1) {
        iVar2 = _AIL_3D_sample_status_4(*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + local_10 * 4));
        if (((iVar2 == 4) && (*param_1 == *(int *)(*(int *)(in_ECX + 500) + local_10 * 0x28))) &&
           (param_1[1] == *(int *)(*(int *)(in_ECX + 500) + 4 + local_10 * 0x28))) {
          return -1;
        }
      }
      cVar1 = FUN_0055faf7();
      if (cVar1 == -1) {
        iVar2 = -1;
      }
      else {
        FUN_0055fbd8();
        local_14 = FUN_0055fb6f(param_1);
        if (local_14 == -1) {
          iVar2 = FUN_0055fca4(param_1,(int)cVar1);
          if (iVar2 == -1) {
            return -1;
          }
          local_8 = (char)iVar2;
          local_14 = local_8;
        }
        else {
          _AIL_set_3D_sample_file_8
                    (*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + cVar1 * 4),
                     *(undefined4 *)(*(int *)(in_ECX + 0x1f8) + 4 + local_14 * 0xc));
        }
        _AIL_set_3D_sample_loop_count_8
                  (*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + cVar1 * 4),param_1[4]);
        _AIL_set_3D_sample_volume_8
                  (*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + cVar1 * 4),param_1[2]);
        _AIL_set_3D_position_16
                  (*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + cVar1 * 4),param_1[7],param_1[8],
                   param_1[9]);
        *(int *)(*(int *)(in_ECX + 500) + 0x18 + cVar1 * 0x28) = (int)local_14;
        _AIL_start_3D_sample_4(*(undefined4 *)(*(int *)(in_ECX + 0x1f0) + cVar1 * 4));
        iVar2 = (int)cVar1;
      }
    }
  }
  else {
    iVar2 = -1;
  }
  return iVar2;
}
