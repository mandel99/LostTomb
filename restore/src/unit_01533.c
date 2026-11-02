#include "restore_types.h"
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
extern int FUN_004bcd90();

int FUN_004bcd60(int param_1,undefined4 param_2)

{
  return FUN_004bcd90((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
               (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],param_2);
}
