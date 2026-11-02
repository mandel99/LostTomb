#include "restore_types.h"
#define DAT_005c738c (*(undefined4 *)0x005c738cu)
#define DAT_006fdc58 (*(undefined4 *)0x006fdc58u)
#define DAT_007056ec (*(undefined *)0x007056ecu)
extern int FUN_00404120();

void FUN_00417a70(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  puVar3 = &DAT_006fdc58;
  puVar2 = &DAT_005c738c;
  do {
    DAT_007056ec = *puVar2;
    uVar1 = FUN_00404120();
    *puVar3 = uVar1;
    puVar2 = puVar2 + 9;
    puVar3 = puVar3 + 1;
  } while ((int)puVar2 < 0x5c753c);
  return;
}
