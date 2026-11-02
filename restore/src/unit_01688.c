#include "restore_types.h"
#define DAT_00e20794 (*(int32_t *)0x00e20794u)
#define DAT_00e2079c (*(uint8_t *)0x00e2079cu)
#define DAT_00e207ec (*(uint8_t *)0x00e207ecu)
#define DAT_00e2083c (*(uint8_t *)0x00e2083cu)
#define DAT_00e20850 (*(uint8_t *)0x00e20850u)
#define DAT_00e208a0 (*(uint8_t *)0x00e208a0u)
#define DAT_00e208f0 (*(uint8_t *)0x00e208f0u)
#define DAT_00e20940 (*(uint8_t *)0x00e20940u)
#define DAT_00e20994 (*(int32_t *)0x00e20994u)

void FUN_004cc560(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined1 param_5,undefined4 param_6)

{
  *(undefined4 *)(&DAT_00e208f0 + param_1 * 4) = param_3;
  *(undefined4 *)(&DAT_00e20850 + param_1 * 4) = param_4;
  *(undefined4 *)(&DAT_00e20940 + param_1 * 4) = param_6;
  (&DAT_00e2083c)[param_1] = param_5;
  *(undefined4 *)(&DAT_00e208a0 + param_1 * 4) = 0;
  *(undefined4 *)(&DAT_00e207ec + param_1 * 4) = 0;
  *(undefined4 *)(&DAT_00e2079c + param_1 * 4) = param_2;
  DAT_00e20994 = 0;
  DAT_00e20794 = 0;
  return;
}
