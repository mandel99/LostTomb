#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00bc9738 (*(undefined *)0x00bc9738u)
#define DAT_00bc973c (*(undefined *)0x00bc973cu)
extern void FUN_0047c480();

void FUN_00416fb0(int param_1)

{
  int *piVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  int in_ECX;
  int iVar6;
  
  iVar4 = param_1;
  iVar5 = param_1 * 0x108;
  sVar2 = *(short *)((int)&DAT_007999dc + iVar5 + 2);
  param_1 = 0;
  sVar3 = *(short *)(&DAT_007999e0 + iVar5);
  DAT_00bc9738 = 1;
  DAT_00bc973c = 1;
  piVar1 = (int *)(in_ECX + 0x54 + (short)(&DAT_007999ea)[iVar4 * 0x84] * 0x34);
  iVar5 = *(int *)(in_ECX + 0x54 + (short)(&DAT_007999ea)[iVar4 * 0x84] * 0x34);
  if (0 < iVar5) {
    do {
      iVar6 = 0;
      if (0 < iVar5) {
        do {
          FUN_0047c480(iVar4,param_1 + sVar2,iVar6 + sVar3);
          iVar6 = iVar6 + 1;
        } while (iVar6 < *piVar1);
      }
      iVar5 = *piVar1;
      param_1 = param_1 + 1;
    } while (param_1 < iVar5);
  }
  return;
}
