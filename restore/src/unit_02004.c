#include "restore_types.h"
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)

void FUN_004e6bd0(int param_1)

{
  int *in_ECX;
  int iVar1;
  
  if ((int)(&DAT_00799a30)[param_1 * 0x42] < 9) {
    iVar1 = (&DAT_00799a30)[param_1 * 0x42] + 1;
    (&DAT_00799a30)[param_1 * 0x42] = iVar1;
    if (iVar1 == 9) {
      *(uint *)(&DAT_00799a3c + param_1 * 0x84) = *(uint *)(&DAT_00799a3c + param_1 * 0x84) | 1;
    }
    (**(code **)(*in_ECX + 0x18))(param_1);
  }
  return;
}
