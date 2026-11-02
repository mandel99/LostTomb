#include "restore_types.h"
#include "sound_initialization.h"

int FUN_0055f0e8(RestoreSound *self,const int32_t *param_1)

{
  char cVar1;
  int iVar2;
  uintptr_t in_ECX=(uintptr_t)self;
  char local_14;
  int local_10;
  char local_8;
  
  if (*(int *)(in_ECX + 0x200) == 0) {
    if ((param_1[1] < 0) || (*(int *)(in_ECX + 4) + -1 < param_1[1])) {
      iVar2 = -1;
    }
    else {
      for (local_10 = 0; local_10 < *(int *)(in_ECX + 0x198); local_10 = local_10 + 1) {
        iVar2 = _AIL_sample_status_4(*(undefined4 *)(*(int *)(in_ECX + 0x19c) + local_10 * 4));
        if (((iVar2 == 4) && (*param_1 == *(int *)(*(int *)(in_ECX + 0x1a8) + local_10 * 0x28))) &&
           (param_1[1] == *(int *)(*(int *)(in_ECX + 0x1a8) + 4 + local_10 * 0x28))) {
          return -1;
        }
      }
      cVar1 = FUN_0055f02d(in_ECX);
      if (cVar1 == -1) {
        iVar2 = -1;
      }
      else {
        FUN_0055ecf3(in_ECX);
        local_14 = FUN_0055f07f(in_ECX,param_1);
        if (local_14 == -1) {
          iVar2 = FUN_0055edd0(in_ECX,param_1,(int)cVar1);
          if (iVar2 == -1) {
            return -1;
          }
          local_8 = (char)iVar2;
          local_14 = local_8;
        }
        else {
          _AIL_set_sample_file_12
                    (*(undefined4 *)(*(int *)(in_ECX + 0x19c) + cVar1 * 4),
                     *(undefined4 *)(*(int *)(in_ECX + 0x1ac) + 4 + local_14 * 0xc),0);
        }
        _AIL_set_sample_loop_count_8
                  (*(undefined4 *)(*(int *)(in_ECX + 0x19c) + cVar1 * 4),param_1[4]);
        _AIL_set_sample_pan_8(*(undefined4 *)(*(int *)(in_ECX + 0x19c) + cVar1 * 4),param_1[3]);
        _AIL_set_sample_volume_8(*(undefined4 *)(*(int *)(in_ECX + 0x19c) + cVar1 * 4),param_1[2]);
        *(int *)(*(int *)(in_ECX + 0x1a8) + 0x18 + cVar1 * 0x28) = (int)local_14;
        if (*(int *)(in_ECX + 0x168) == 0) {
          _AIL_set_sample_reverb_16(*(undefined4 *)(*(int *)(in_ECX + 0x19c) + cVar1 * 4),0,0,0);
        }
        else {
          _AIL_set_sample_reverb_16
                    (*(undefined4 *)(*(int *)(in_ECX + 0x19c) + cVar1 * 4),
                     *(undefined4 *)(in_ECX + 0x16c),*(undefined4 *)(in_ECX + 0x170),
                     *(undefined4 *)(in_ECX + 0x174));
        }
        _AIL_start_sample_4(*(undefined4 *)(*(int *)(in_ECX + 0x19c) + cVar1 * 4));
        iVar2 = (int)cVar1;
      }
    }
  }
  else {
    iVar2 = -1;
  }
  return iVar2;
}
