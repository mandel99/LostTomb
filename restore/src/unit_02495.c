#include "restore_types.h"
#include "sound_streams.h"
#define DAT_00e92dd0 (*(undefined4 *)0x00e92dd0u)
#define DAT_00f55b88 (*(undefined *)0x00f55b88u)
#define DAT_00f55b8c (*(undefined *)0x00f55b8cu)
#define DAT_00f55b90 (*(undefined *)0x00f55b90u)

void FUN_0053b6f0(int param_1)

{
  int iVar1;
  
  if (DAT_00f55b88 == 0) {
    if (param_1 == 0) {
      DAT_00f55b8c = 0;
      return;
    }
  }
  else if (*(int *)(&DAT_00e92dd0 + param_1 * 4) == 0) {
    if (param_1 == 0) {
      DAT_00f55b8c = 0;
      return;
    }
  }
  else {
    iVar1 = FUN_0055f5a1(RESTORE_SOUND,param_1);
    if (iVar1 == 0) {
      if (param_1 == 0) {
        DAT_00f55b8c = 0;
        return;
      }
    }
    else {
      iVar1 = FUN_0055f5f4(RESTORE_SOUND,param_1);
      if (-1 < iVar1 + -5) {
        FUN_0055f639(RESTORE_SOUND,param_1,iVar1 + -5);
        return;
      }
      FUN_0055f561(RESTORE_SOUND,param_1);
      if (param_1 == 0) {
        DAT_00f55b8c = 0;
        return;
      }
    }
  }
  if (param_1 != 1) {
    return;
  }
  DAT_00f55b90 = 0;
  return;
}
