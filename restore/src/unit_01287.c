#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
extern void FUN_00488c20();

void FUN_00488e60(int param_1,undefined4 param_2)

{
  FUN_00488c20(param_1,(int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2),
               (int)*(short *)(&DAT_007999e0 + param_1 * 0x108),param_2);
  return;
}
