#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00bc9738 (*(undefined *)0x00bc9738u)
#define DAT_00bc973c (*(undefined *)0x00bc973cu)
extern void FUN_0047c480();

void FUN_0040d890(int param_1)

{
  int *piVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int in_ECX;
  int iVar5;
  int iVar6;
  
  iVar3 = param_1;
  iVar5 = (int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2);
  sVar2 = *(short *)(&DAT_007999e0 + param_1 * 0x108);
  piVar1 = (int *)(in_ECX + 0x54 + (short)(&DAT_007999ea)[param_1 * 0x84] * 0x34);
  DAT_00bc9738 = *(undefined4 *)(in_ECX + 0x54 + (short)(&DAT_007999ea)[param_1 * 0x84] * 0x34);
  DAT_00bc973c = *piVar1;
  FUN_0047c480(param_1,iVar5,(int)sVar2);
  iVar4 = *piVar1;
  param_1 = 0;
  if (0 < iVar4) {
    do {
      iVar6 = 0;
      if (0 < iVar4) {
        do {
          FUN_0047c480(iVar3,param_1 + iVar5,iVar6 + sVar2);
          iVar6 = iVar6 + 1;
        } while (iVar6 < *piVar1);
      }
      iVar4 = *piVar1;
      param_1 = param_1 + 1;
    } while (param_1 < iVar4);
  }
  return;
}
