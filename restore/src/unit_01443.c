#include "restore_types.h"
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd67a (*(undefined4 *)0x00bcd67au)
#define DAT_00bcd67e (*(undefined4 *)0x00bcd67eu)
#define DAT_00bcd680 (*(undefined4 *)0x00bcd680u)

bool FUN_004b46b0(int param_1)

{
  int iVar1;
  
  iVar1 = (int)*(short *)(&DAT_00bcd67a + param_1 * 0x184);
  if (iVar1 < 1) {
    return false;
  }
  if ((&DAT_00bcd606)[iVar1 * 0x184] != '\x01') {
    return false;
  }
  if ((&DAT_00bcd630)[iVar1 * 0x184] == '\x04') {
    return false;
  }
  return (&DAT_00bcd67e)[iVar1 * 0xc2] == *(short *)(&DAT_00bcd680 + param_1 * 0x184);
}
