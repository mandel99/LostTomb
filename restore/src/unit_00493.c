#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799abc (*(undefined4 *)0x00799abcu)

int FUN_00433e80(int param_1)

{
  short sVar1;
  
  sVar1 = (&DAT_007999e8)[param_1 * 0x84];
  if ((((sVar1 != 0x37) && (sVar1 != 0xb8)) && (sVar1 != 99)) &&
     (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0')) {
    return (int)(&DAT_00799abc)[param_1 * 0x42] / 100;
  }
  return 0;
}
