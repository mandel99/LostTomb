#include "restore_types.h"
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
extern int FUN_004bcf70();

int FUN_004bcf40(int param_1,undefined4 param_2)

{
  return FUN_004bcf70((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
               (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],param_2);
}
