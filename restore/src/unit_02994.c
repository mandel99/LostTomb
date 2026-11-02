#include "restore_types.h"
#define DAT_00604c62 (*(undefined4 *)0x00604c62u)
#define DAT_01266840 (*(undefined4 *)0x01266840u)

undefined4 FUN_00569800(byte param_1,uint param_2,byte param_3)

{
  if ((*(byte *)((int)&DAT_01266840 + param_1 + 1) & param_3) == 0) {
    if (param_2 == 0) {
      param_2 = 0;
    }
    else {
      param_2 = *(ushort *)(&DAT_00604c62 + (uint)param_1 * 2) & param_2;
    }
    if (param_2 == 0) {
      return 0;
    }
  }
  return 1;
}
