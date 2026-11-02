#include "restore_types.h"
#define DAT_005cc9e0 (*(undefined4 *)0x005cc9e0u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

undefined4 FUN_00480770(int param_1,int param_2,int param_3)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = 0;
  piVar1 = (int *)(&DAT_005cc9e0 + param_3 * 0x60);
  while( true ) {
    if (*piVar1 == 0) {
      return 0;
    }
    if (((&DAT_00af598c)[*piVar1 + DAT_00bca2d8 + param_2 * 0xe4 + param_1] & 0x4000) != 0) break;
    iVar2 = iVar2 + 1;
    piVar1 = piVar1 + 1;
    if (0x17 < iVar2) {
      return 0;
    }
  }
  return 1;
}
