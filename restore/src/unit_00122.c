#include "restore_types.h"
#define DAT_00d3b5bc (*(undefined *)0x00d3b5bcu)
#define DAT_00d3b5be (*(undefined *)0x00d3b5beu)
#define DAT_00d3b5dc (*(undefined *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(undefined *)0x00d3b5e0u)
#define DAT_00e38d94 (*(undefined *)0x00e38d94u)
#define DAT_00e92e18 (*(undefined *)0x00e92e18u)

void FUN_0040f9f0(undefined4 *param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  
  puVar3 = (undefined4 *)(DAT_00e92e18 + DAT_00d3b5e0 * 2 + DAT_00d3b5dc * DAT_00e38d94);
  iVar2 = (int)DAT_00d3b5bc;
  do {
    for (iVar1 = DAT_00d3b5be * 2; iVar1 != 0; iVar1 = iVar1 + -1) {
      *param_1 = *puVar3;
      puVar3 = puVar3 + 1;
      param_1 = param_1 + 1;
    }
    puVar3 = (undefined4 *)((int)puVar3 + param_2);
    param_1 = (undefined4 *)((int)param_1 + param_3);
    iVar2 = iVar2 + -1;
  } while (0 < iVar2);
  return;
}
