#include "restore_types.h"
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca34c (*(undefined *)0x00bca34cu)
extern int FUN_004cde60();

int FUN_004883d0(int param_1)

{
  return FUN_004cde60((int)(short)(&DAT_00799a2c)[param_1 * 0x84],DAT_00bca34c);
}
