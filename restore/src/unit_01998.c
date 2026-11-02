#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
extern void FUN_00477a40();
extern void FUN_0047c480();

void FUN_004e6660(int param_1)

{
  int iVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int in_ECX;
  int iVar5;
  int iVar6;
  int local_8;
  
  iVar3 = param_1;
  local_8 = 0;
  iVar5 = (int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2);
  sVar2 = *(short *)(&DAT_007999e0 + param_1 * 0x108);
  iVar1 = in_ECX + (short)(&DAT_007999ea)[param_1 * 0x84] * 0x34;
  iVar4 = *(int *)(in_ECX + 0x54 + (short)(&DAT_007999ea)[param_1 * 0x84] * 0x34);
  if (0 < iVar4) {
    do {
      param_1 = 0;
      iVar6 = (int)sVar2;
      if (0 < iVar4) {
        do {
          FUN_0047c480(iVar3,iVar5,iVar6);
          if (*(int *)(iVar1 + 0x58) == 0) {
            FUN_00477a40(iVar5,iVar6,2);
          }
          param_1 = param_1 + 1;
          iVar6 = iVar6 + 1;
        } while (param_1 < *(int *)(iVar1 + 0x54));
      }
      iVar4 = *(int *)(iVar1 + 0x54);
      local_8 = local_8 + 1;
      iVar5 = iVar5 + 1;
    } while (local_8 < iVar4);
  }
  return;
}
