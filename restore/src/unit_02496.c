#include "restore_types.h"
#include "sound_streams.h"
#define DAT_00e38ec8 (*(undefined *)0x00e38ec8u)
#define DAT_00e38ecc (*(undefined *)0x00e38eccu)
#define DAT_00e38ed0 (*(undefined *)0x00e38ed0u)
#define DAT_00e92dd0 (*(undefined4 *)0x00e92dd0u)
#define DAT_00f55b88 (*(undefined *)0x00f55b88u)

void FUN_0053b790(int param_1)

{
  int iVar1;
  
  if ((DAT_00f55b88 != 0) && (*(int *)(&DAT_00e92dd0 + param_1 * 4) != 0)) {
    iVar1 = FUN_0055f5a1(RESTORE_SOUND,param_1);
    if (iVar1 != 0) {
      iVar1 = FUN_0055f5f4(RESTORE_SOUND,param_1);
      iVar1 = iVar1 + 5;
      if (param_1 == 0) {
        if (DAT_00e38ec8 < iVar1) {
          iVar1 = DAT_00e38ec8;
        }
      }
      else if (param_1 == 1) {
        if (DAT_00e38ecc < iVar1) {
          FUN_0055f639(RESTORE_SOUND,1,DAT_00e38ecc);
          return;
        }
      }
      else if ((param_1 == 2) && (DAT_00e38ed0 < iVar1)) {
        FUN_0055f639(RESTORE_SOUND,2,DAT_00e38ed0);
        return;
      }
      FUN_0055f639(RESTORE_SOUND,param_1,iVar1);
    }
  }
  return;
}
