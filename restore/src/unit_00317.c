#include "restore_types.h"
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)

void FUN_0041fd80(int param_1)

{
  int in_ECX;
  
  *(short *)(in_ECX + 0x12) = (short)param_1;
  if (param_1 != 0) {
    *(undefined1 *)(in_ECX + 0x14) = (&DAT_00bcd630)[param_1 * 0x184];
  }
  return;
}
