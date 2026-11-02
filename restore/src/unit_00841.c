#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00ea3f8c (*(undefined4 *)0x00ea3f8cu)

void FUN_00458f00(int param_1,int param_2)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = 0;
  iVar3 = (int)*(short *)(&DAT_00ea3f8c + param_1 * 0x49d8);
  iVar4 = 1;
  do {
    if (param_2 <= iVar2) {
      return;
    }
    iVar3 = iVar3 + 1;
    if (3999 < iVar3) {
      iVar3 = 1;
    }
    if (((&DAT_007999d8)[iVar3 * 0x108] == '\x01') &&
       (*(char *)((int)&DAT_007999dc + iVar3 * 0x108 + 1) != '\0')) {
      sVar1 = (&DAT_007999f4)[iVar3 * 0x84];
      *(short *)(&DAT_00ea3f8c + param_1 * 0x49d8) = (short)iVar3;
      if (0 < sVar1) {
        iVar2 = iVar2 + 1;
        (&DAT_007999f4)[iVar3 * 0x84] = sVar1 + -1;
      }
    }
    iVar4 = iVar4 + 1;
  } while (iVar4 < 16000);
  return;
}
