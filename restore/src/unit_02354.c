#include "restore_types.h"
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define _DAT_00f53f30 (*(undefined4 *)0x00f53f30u)
extern void FUN_004dddc0();
extern void FUN_0051e990();
extern void FUN_00520000();

void FUN_0051ffd0(void)

{
  FUN_0051e990();
  FUN_004dddc0();
  if (1 < DAT_00e38dca) {
    _DAT_00f53f30 = 0;
    FUN_00520000();
    return;
  }
  return;
}
