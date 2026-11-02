#include "restore_types.h"
#define DAT_005c7388 (*(undefined4 *)0x005c7388u)
#define DAT_005c738c (*(undefined4 *)0x005c738cu)
#define DAT_005c75c8 (*(undefined4 *)0x005c75c8u)
#define DAT_005c75cc (*(undefined4 *)0x005c75ccu)
#define DAT_007056ec (*(undefined *)0x007056ecu)
extern int FUN_00404120();

void FUN_004051a0(void)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 0;
  do {
    (&DAT_005c7388)[iVar2] = 1;
    DAT_007056ec = *(undefined4 *)((int)&DAT_005c738c + iVar2);
    iVar1 = FUN_00404120();
    if (iVar1 < 1) {
      (&DAT_005c7388)[iVar2] = 0;
    }
    (&DAT_005c75c8)[iVar2] = 1;
    DAT_007056ec = *(undefined4 *)((int)&DAT_005c75cc + iVar2);
    iVar1 = FUN_00404120();
    if (iVar1 < 1) {
      (&DAT_005c75c8)[iVar2] = 0;
    }
    iVar2 = iVar2 + 0x24;
  } while (iVar2 < 0x1b0);
  return;
}
