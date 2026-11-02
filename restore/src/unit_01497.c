#include "restore_types.h"
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)

int FUN_004b8460(uint param_1)

{
  char *pcVar1;
  
  DAT_00d056f8 = 1;
  pcVar1 = &DAT_00bcd786;
  while (((pcVar1[4] == '\0' || (*pcVar1 != 'M')) || ((byte)pcVar1[0x88] != param_1))) {
    pcVar1 = pcVar1 + 0x184;
    DAT_00d056f8 = DAT_00d056f8 + 1;
    if (0xc8ad41 < (int)pcVar1) {
      return 0;
    }
  }
  return DAT_00d056f8;
}
