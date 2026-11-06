#include "restore_types.h"
#define DAT_007090d0 (*(uint8_t *)0x007090d0u)
#define DAT_007090d4 (*(uint8_t *)0x007090d4u)
#define DAT_007090ec (*(uint8_t *)0x007090ecu)
#define DAT_007090f4 (*(uint8_t *)0x007090f4u)
#define DAT_007090fc (*(uint8_t *)0x007090fcu)
#define DAT_00709100 (*(uint8_t *)0x00709100u)
#define DAT_00bcd602 (*(uint8_t *)0x00bcd602u)
#define DAT_00bcd71c (*(uint8_t *)0x00bcd71cu)
#define DAT_00bcd720 (*(uint8_t *)0x00bcd720u)
#define DAT_00bcd724 (*(uint8_t *)0x00bcd724u)
#define DAT_00bcd728 (*(uint8_t *)0x00bcd728u)
#define DAT_00bcd72c (*(uint8_t *)0x00bcd72cu)
#define DAT_00bcd730 (*(uint8_t *)0x00bcd730u)
#define DAT_00bcd734 (*(uint8_t *)0x00bcd734u)
#define DAT_00bcd738 (*(uint8_t *)0x00bcd738u)
#define DAT_00bcd73c (*(uint8_t *)0x00bcd73cu)
#define DAT_00bcd740 (*(uint8_t *)0x00bcd740u)
#define DAT_00bcd748 (*(uint8_t *)0x00bcd748u)
#define DAT_00bcd74c (*(uint8_t *)0x00bcd74cu)
#define DAT_00bcd750 (*(uint8_t *)0x00bcd750u)
#define DAT_00ea6210 (*(uint8_t *)0x00ea6210u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
extern int FUN_004cde40();

void FUN_00446650(int param_1)

{
  undefined4 uVar1;
  
  param_1 = param_1 * 0x184;
  *(undefined4 *)(&DAT_00bcd71c + param_1) = 0;
  *(undefined4 *)(&DAT_00bcd720 + param_1) =
       *(undefined4 *)(&DAT_007090d0 + (uint)(byte)(&DAT_00bcd602)[param_1] * 0x38);
  *(undefined4 *)(&DAT_00bcd724 + param_1) =
       *(undefined4 *)(&DAT_007090d0 + (uint)(byte)(&DAT_00bcd602)[param_1] * 0x38);
  (&DAT_00bcd728)[param_1] = 0;
  *(undefined4 *)(&DAT_00bcd730 + param_1) = 0;
  *(undefined4 *)(&DAT_00bcd72c + param_1) = 0;
  *(undefined4 *)(&DAT_00bcd734 + param_1) =
       *(undefined4 *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[param_1] * 0x38);
  *(undefined4 *)(&DAT_00bcd738 + param_1) =
       *(undefined4 *)(&DAT_007090f4 + (uint)(byte)(&DAT_00bcd602)[param_1] * 0x38);
  *(undefined4 *)(&DAT_00bcd73c + param_1) =
       *(undefined4 *)(&DAT_007090fc + (uint)(byte)(&DAT_00bcd602)[param_1] * 0x38);
  *(undefined4 *)(&DAT_00bcd740 + param_1) =
       *(undefined4 *)(&DAT_00709100 + (uint)(byte)(&DAT_00bcd602)[param_1] * 0x38);
  *(undefined4 *)(&DAT_00bcd748 + param_1) =
       *(undefined4 *)(&DAT_007090d4 + (uint)(byte)(&DAT_00bcd602)[param_1] * 0x38);
  (&DAT_00bcd74c)[param_1] = 0;
  uVar1 = FUN_004cde40(*(undefined4 *)(&DAT_007090d4 + (uint)(byte)(&DAT_00bcd602)[param_1] * 0x38),
                       0x14);
  *(undefined4 *)(&DAT_00bcd750 + param_1) = uVar1;
  *(uint32_t *)(&DAT_00ea6210 + DAT_00eaac28 * 0x49d8u) =
       *(uint32_t *)(&DAT_00ea6210 + DAT_00eaac28 * 0x49d8u) + 1u;
  return;
}
