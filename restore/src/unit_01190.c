#include "restore_types.h"
#define DAT_005cc9e0 (*(undefined4 *)0x005cc9e0u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)

undefined4 FUN_00481420(int param_1,int param_2,int param_3)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  piVar1 = (int *)(&DAT_005cc9e0 + param_3 * 0x60);
  while( true ) {
    if (*piVar1 == 0) {
      return 0;
    }
    iVar2 = *piVar1 + DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    if (((*(byte *)(&DAT_00af598c + iVar2) & 4) != 0) && ((&DAT_00eed760)[iVar2] != 0)) break;
    iVar3 = iVar3 + 1;
    piVar1 = piVar1 + 1;
    if (0x17 < iVar3) {
      return 0;
    }
  }
  return 1;
}
