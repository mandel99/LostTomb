#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f5d08 (*(undefined4 *)0x005f5d08u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern bool FUN_004ecea0();
extern void FUN_004ed360();
extern void FUN_004edf50();

void FUN_004ee4a0(int param_1)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  uVar2 = (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84);
  iVar4 = DAT_00bca2e0 / 2;
  iVar5 = param_1;
  while( true ) {
    if (DAT_005e4ef4 == 5) {
      iVar3 = *(int *)(&DAT_0056f7ec + uVar2 * 4);
    }
    else {
      iVar3 = *(int *)(&DAT_0056f7d8 + uVar2 * 4);
    }
    if (iVar3 + param_1 <= iVar5) break;
    FUN_004edf50(iVar5,iVar4);
    iVar5 = iVar5 + 1;
  }
  cVar1 = FUN_004ecea0(uVar2);
  if (cVar1 != '\0') {
    iVar5 = (*(int *)(&DAT_005f5d08 + uVar2 * 4) + *(int *)(&DAT_007999e4 + param_1 * 0x108)) -
            DAT_00bca2d8;
    FUN_004ed360(iVar5 % 0xe4,iVar5 / 0xe4,4,0);
  }
  return;
}
