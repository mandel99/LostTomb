#include "restore_types.h"
#define DAT_0070ce14 (*(undefined4 *)0x0070ce14u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_007999f6 (*(undefined4 *)0x007999f6u)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_00ea3f88 (*(undefined4 *)0x00ea3f88u)

void FUN_00458e20(int param_1,int param_2)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar2 = 0;
  iVar3 = param_1 * 0x49d8;
  iVar5 = *(int *)(&DAT_00ea3f88 + iVar3);
  param_1 = 1;
  do {
    if (param_2 <= iVar2) {
      return;
    }
    iVar5 = iVar5 + 1;
    if (3999 < iVar5) {
      iVar5 = 1;
    }
    if (((((&DAT_007999d8)[iVar5 * 0x108] == '\x01') &&
         (*(char *)((int)&DAT_007999dc + iVar5 * 0x108 + 1) != '\0')) &&
        (0 < (short)(&DAT_007999f8)[iVar5 * 0x84])) &&
       (sVar1 = (&DAT_007999f4)[iVar5 * 0x84], 0 < sVar1)) {
      *(int *)(&DAT_00ea3f88 + iVar3) = iVar5;
      iVar4 = *(int *)(&DAT_0070ce14 + (short)(&DAT_007999ea)[iVar5 * 0x84] * 0x58);
      if (*(char *)(&DAT_007999dc + iVar5 * 0x42) != '\0') {
        iVar4 = iVar4 << 2;
      }
      if (iVar4 != sVar1 && -1 < iVar4 - sVar1) {
        iVar2 = iVar2 + 1;
        (&DAT_007999f4)[iVar5 * 0x84] = sVar1 + 1;
        (&DAT_007999f6)[iVar5 * 0x84] = (short)iVar4 - (sVar1 + 1);
      }
    }
    param_1 = param_1 + 1;
  } while (param_1 < 4000);
  return;
}
