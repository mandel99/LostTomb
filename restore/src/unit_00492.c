#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799ac0 (*(undefined4 *)0x00799ac0u)

int FUN_00433e40(int param_1)

{
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0') {
    return (int)(&DAT_00799ac0)[param_1 * 0x42] / 100;
  }
  return 0;
}
