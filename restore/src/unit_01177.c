#include "restore_types.h"
#define DAT_005cc9e0 (*(undefined4 *)0x005cc9e0u)
#define DAT_005cce98 (*(undefined4 *)0x005cce98u)
#define DAT_005cce9c (*(undefined4 *)0x005cce9cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern void FUN_0047db00();
extern int FUN_00484f40();

undefined4 FUN_004806c0(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  iVar3 = 0;
  iVar2 = DAT_00bca2d8 + param_2 * 0xe4;
  piVar4 = (int *)(&DAT_005cc9e0 + param_3 * 0x60);
  do {
    if (*piVar4 == 0) {
      return 0;
    }
    iVar1 = *piVar4 + iVar2 + param_1;
    if ((((&DAT_00af598c)[iVar1] & 4) != 0) && (((&DAT_00af598c)[iVar1] & 8) == 0)) {
      DAT_00bc9768 = iVar1;
      FUN_0047db00(4);
      iVar1 = FUN_00484f40();
      if (7 < iVar1) {
        iVar2 = (iVar3 + param_3 * 0x18) * 8;
        _DAT_00d3b4e4 = *(int *)(&DAT_005cce98 + iVar2) + param_1;
        _DAT_00d3b4e0 = *(int *)(&DAT_005cce9c + iVar2) + param_2;
        return 1;
      }
    }
    iVar3 = iVar3 + 1;
    piVar4 = piVar4 + 1;
  } while (iVar3 < 0x18);
  return 0;
}
