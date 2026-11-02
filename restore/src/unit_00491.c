#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799ad4 (*(undefined4 *)0x00799ad4u)

int FUN_00433e00(int param_1)

{
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0') {
    return *(int *)(&DAT_00799ad4 + param_1 * 0x108) / 100;
  }
  return 0;
}
