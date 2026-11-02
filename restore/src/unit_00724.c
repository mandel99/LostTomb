#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

bool FUN_0044bd20(int param_1,int param_2)

{
  return ((&DAT_00af598c)[DAT_00bca2d8 + param_2 * 0xe4 + param_1] & 0x206) == 0;
}
