#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)

undefined4 FUN_004e6ae0(int param_1,int *param_2,int *param_3)

{
  short sVar1;
  
  *param_2 = *(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2) + 2;
  sVar1 = *(short *)(&DAT_007999e0 + param_1 * 0x108);
  *param_3 = sVar1 + 6;
  return CONCAT31((int3)((uint)(sVar1 + 6) >> 8),1);
}
