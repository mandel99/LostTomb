#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f53e70 (*(undefined4 *)0x00f53e70u)
extern undefined4 FUN_005013a0();
extern undefined4 FUN_00502780();

int FUN_005027b0(void)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  
  sVar1 = (&DAT_007999e8)[DAT_00eadd6c * 0x84];
  if ((sVar1 == 0x48) || (sVar1 == 0xc1)) {
    iVar3 = FUN_00502780();
    if (iVar3 < DAT_00f53e6c) {
      iVar3 = FUN_00502780();
      return iVar3;
    }
    return DAT_00f53e6c;
  }
  if ((sVar1 != 0x46) && (sVar1 != 0xbe)) {
    return DAT_00f53dd8;
  }
  iVar3 = 0;
  iVar4 = 0;
  if (0 < DAT_00f53e6c) {
    puVar5 = &DAT_00f53e70;
    do {
      iVar2 = FUN_005013a0(*puVar5);
      if (iVar2 != 0) {
        iVar3 = iVar3 + 1;
      }
      iVar4 = iVar4 + 1;
      puVar5 = puVar5 + 1;
    } while (iVar4 < DAT_00f53e6c);
    return iVar3;
  }
  return 0;
}
