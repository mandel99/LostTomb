#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
extern uint FUN_00489470();

uint FUN_004894a0(int param_1)

{
  return FUN_00489470((int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2),
               (int)*(short *)(&DAT_007999e0 + param_1 * 0x108),3,3);
}
