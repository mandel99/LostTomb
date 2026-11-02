#include "restore_types.h"
#define DAT_007090d0 (*(undefined4 *)0x007090d0u)
#define DAT_007090ec (*(undefined4 *)0x007090ecu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd71c (*(undefined4 *)0x00bcd71cu)
#define DAT_00bcd720 (*(undefined4 *)0x00bcd720u)
#define DAT_00bcd724 (*(undefined4 *)0x00bcd724u)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
#define DAT_00bcd734 (*(undefined4 *)0x00bcd734u)
#define DAT_00bcd74c (*(undefined4 *)0x00bcd74cu)

void FUN_004620c0(int param_1)

{
  param_1 = param_1 * 0x184;
  *(undefined4 *)(&DAT_00bcd71c + param_1) = 0;
  *(undefined4 *)(&DAT_00bcd720 + param_1) =
       *(undefined4 *)(&DAT_007090d0 + (uint)(byte)(&DAT_00bcd602)[param_1] * 0x38);
  *(undefined4 *)(&DAT_00bcd724 + param_1) =
       *(undefined4 *)(&DAT_007090d0 + (uint)(byte)(&DAT_00bcd602)[param_1] * 0x38);
  (&DAT_00bcd728)[param_1] = 0;
  *(undefined4 *)(&DAT_00bcd734 + param_1) =
       *(undefined4 *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[param_1] * 0x38);
  (&DAT_00bcd74c)[param_1] = 0;
  return;
}
