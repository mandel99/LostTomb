#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)

void FUN_00417640(int param_1)

{
  int *in_ECX;
  int iVar1;
  
  param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  if ((int)(&DAT_00799a30)[param_1 * 0x42] < 0x13) {
    iVar1 = (&DAT_00799a30)[param_1 * 0x42] + 1;
    (&DAT_00799a30)[param_1 * 0x42] = iVar1;
    if (iVar1 == 0x13) {
      *(uint *)(&DAT_00799a3c + param_1 * 0x84) = *(uint *)(&DAT_00799a3c + param_1 * 0x84) | 1;
    }
    iVar1 = 0;
    do {
      (**(code **)(*in_ECX + 0x18))(iVar1 + param_1);
      iVar1 = iVar1 + 1;
    } while (iVar1 < 9);
  }
  return;
}
