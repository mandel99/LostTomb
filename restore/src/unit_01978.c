#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)

int FUN_004e5a30(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = (-(uint)((&DAT_007999e8)[param_1 * 0x84] != 0xcf) & 8) + 10;
  iVar1 = (int)((*(byte *)((int)&DAT_00799a2c + param_1 * 0x108 + 1) - 0x21) * iVar2) / 0x42 + 1;
  if (iVar2 <= iVar1) {
    iVar1 = iVar2;
  }
  return iVar1;
}
