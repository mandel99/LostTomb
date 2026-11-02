#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f5d08 (*(undefined4 *)0x005f5d08u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define PTR_DAT_005f6cd0 (*(unsigned char * *)0x005f6cd0u)
#define PTR_DAT_005f6d58 (*(unsigned char * *)0x005f6d58u)
extern bool FUN_004ecea0();
extern void FUN_004ed360();
extern void FUN_004edf50();

void FUN_004f3c90(int param_1)

{
  int iVar1;
  char cVar2;
  int iVar3;
  uint uVar4;
  undefined **ppuVar5;
  int iVar6;
  int iVar7;
  
  uVar4 = (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84);
  iVar6 = DAT_00bca2e0 / 2;
  ppuVar5 = &PTR_DAT_005f6d58;
  if (DAT_005e4ef4 != 5) {
    ppuVar5 = &PTR_DAT_005f6cd0;
  }
  iVar7 = 0;
  if (0 < (int)ppuVar5[uVar4 * 2 + 1]) {
    do {
      iVar1 = *(int *)(ppuVar5[uVar4 * 2] + iVar7 * 8);
      iVar3 = (iVar1 + param_1) * 0x108;
      *(undefined1 *)((int)&DAT_00799a2c + iVar3 + 1) = 3;
      *(undefined1 *)((int)&DAT_00799a30 + iVar3 + 3) = 0;
      FUN_004edf50(iVar1 + param_1,iVar6,uVar4);
      iVar7 = iVar7 + 1;
    } while (iVar7 < (int)ppuVar5[uVar4 * 2 + 1]);
  }
  cVar2 = FUN_004ecea0(uVar4);
  if (cVar2 != '\0') {
    iVar6 = (*(int *)(&DAT_005f5d08 + uVar4 * 4) + *(int *)(&DAT_007999e4 + param_1 * 0x108)) -
            DAT_00bca2d8;
    FUN_004ed360(iVar6 % 0xe4,iVar6 / 0xe4,2,0);
  }
  return;
}
