#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
extern uint FUN_004893c0();

uint FUN_004894d0(int param_1)

{
  return FUN_004893c0((int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2),
               (int)*(short *)(&DAT_007999e0 + param_1 * 0x108),3,3,4);
}
