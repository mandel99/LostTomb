#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
extern void FUN_004ee4a0();

void FUN_004ee5d0(int param_1)

{
  byte bVar1;
  int iVar2;
  undefined1 *puVar3;
  int iVar4;
  int iVar5;
  
  iVar2 = DAT_005e4ef4;
  iVar5 = 0;
  bVar1 = *(byte *)(&DAT_00799a2c + param_1 * 0x84);
  puVar3 = (undefined1 *)((int)&DAT_00799a2c + param_1 * 0x108 + 1);
  while( true ) {
    if (iVar2 == 5) {
      iVar4 = *(int *)(&DAT_0056f7ec + (uint)bVar1 * 4);
    }
    else {
      iVar4 = *(int *)(&DAT_0056f7d8 + (uint)bVar1 * 4);
    }
    if (iVar4 <= iVar5) break;
    *puVar3 = 0;
    iVar5 = iVar5 + 1;
    puVar3 = puVar3 + 0x108;
  }
  FUN_004ee4a0(param_1);
  return;
}
