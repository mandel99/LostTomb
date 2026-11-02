#include "restore_types.h"
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
extern void FUN_004a5680();
extern void FUN_004b3250();

void FUN_004ae270(int param_1,undefined4 *param_2)

{
  FUN_004a5680(param_1,0);
  *param_2 = 1;
  (&DAT_00bcd65d)[param_1 * 0x184] = 0;
  if (((&DAT_00bcd609)[param_1 * 0x184] == '\t') || ((&DAT_00bcd609)[param_1 * 0x184] == '\n')) {
    FUN_004b3250(param_1);
  }
  return;
}
