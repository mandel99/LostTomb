#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
extern undefined4 FUN_00487dc0();

undefined4 FUN_00487e30(int param_1)

{
  return FUN_00487dc0((int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2),
               (int)*(short *)(&DAT_007999e0 + param_1 * 0x108));
}
