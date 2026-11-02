#include "restore_types.h"
#define DAT_005e2ae4 (*(undefined4 *)0x005e2ae4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
extern void FUN_004e0c80();

void FUN_004df2c0(int param_1)

{
  uint uVar1;
  int *piVar2;
  uint *puVar3;
  
  puVar3 = &DAT_005e2ae4;
  piVar2 = &DAT_00799a30 + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x42;
  do {
    uVar1 = *puVar3;
    puVar3 = puVar3 + 0xd;
    *piVar2 = (((uVar1 & 0xff) < 4) - 1 & 0xffff0003) + 0xffff;
    piVar2[3] = 0xff;
    piVar2[2] = 3;
    *(undefined1 *)(piVar2 + 7) = 1;
    *(undefined2 *)((int)piVar2 + -0x42) = 0xffff;
    piVar2 = piVar2 + 0x42;
  } while ((int)puVar3 < 0x5e4274);
  FUN_004e0c80(param_1);
  return;
}
