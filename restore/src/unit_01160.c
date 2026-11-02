#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)

undefined4 FUN_0047f190(int param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  
  iVar4 = (int)*(short *)(&DAT_00b4e6fc + param_1 * 2);
  if ((iVar4 != 0) && ((&DAT_007999e8)[iVar4 * 0x84] == 0x5a)) {
    piVar3 = &DAT_005cc8c0;
    do {
      iVar2 = 0;
      piVar1 = piVar3;
      do {
        if (iVar4 != *(short *)(&DAT_00b4e6fc + (param_1 + *piVar1) * 2)) {
          return 0;
        }
        iVar2 = iVar2 + 1;
        piVar1 = piVar1 + 2;
      } while (iVar2 < 2);
      piVar3 = piVar3 + 0xc;
    } while ((int)piVar3 < 0x5cc920);
    return 1;
  }
  return 0;
}
