#include "restore_types.h"
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
extern void FUN_00541920();

void FUN_00542dd0(int param_1)

{
  int iVar1;
  
  if ((int)(&DAT_00799a30)[param_1 * 0x42] < 0xd) {
    iVar1 = (&DAT_00799a30)[param_1 * 0x42] + 1;
    (&DAT_00799a30)[param_1 * 0x42] = iVar1;
    if (iVar1 == 0xd) {
      *(uint *)(&DAT_00799a3c + param_1 * 0x84) = *(uint *)(&DAT_00799a3c + param_1 * 0x84) | 1;
    }
    FUN_00541920(param_1);
  }
  return;
}
