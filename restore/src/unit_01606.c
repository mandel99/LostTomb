#include "restore_types.h"
#define DAT_00799a80 (*(undefined4 *)0x00799a80u)

bool FUN_004c52d0(int param_1,uint param_2)

{
  byte bVar1;
  
  bVar1 = *(byte *)((int)&DAT_00799a80 + param_1 * 0x108 + 1);
  if (bVar1 == 3) {
    return true;
  }
  return bVar1 == param_2;
}
