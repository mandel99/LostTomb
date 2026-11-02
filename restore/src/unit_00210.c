#include "restore_types.h"
#define DAT_005c738b (*(undefined4 *)0x005c738bu)
#define DAT_006fdc58 (*(undefined4 *)0x006fdc58u)
#define DAT_007056ec (*(undefined *)0x007056ecu)
extern int FUN_00404120();

void FUN_00417aa0(void)

{
  int iVar1;
  int *piVar2;
  undefined1 *puVar3;
  
  puVar3 = &DAT_005c738b;
  piVar2 = &DAT_006fdc58;
  do {
    DAT_007056ec = *(undefined4 *)(puVar3 + 1);
    iVar1 = FUN_00404120();
    if (*piVar2 < iVar1) {
      *puVar3 = 1;
    }
    piVar2 = piVar2 + 1;
    puVar3 = puVar3 + 0x24;
  } while ((int)piVar2 < 0x6fdc88);
  return;
}
