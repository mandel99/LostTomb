#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)

undefined4 FUN_00401af0(int param_1,int *param_2,int *param_3)

{
  short sVar1;
  int iVar2;
  
  iVar2 = (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x108;
  *param_2 = *(short *)((int)&DAT_007999dc + iVar2 + 2) + 6;
  sVar1 = *(short *)(&DAT_007999e0 + iVar2);
  *param_3 = sVar1 + -8;
  return CONCAT31((int3)((uint)(sVar1 + -8) >> 8),1);
}
