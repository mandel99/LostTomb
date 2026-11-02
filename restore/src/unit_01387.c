#include "restore_types.h"
#define DAT_00bcd758 (*(undefined4 *)0x00bcd758u)
extern void FUN_004b27e0();

void FUN_004a5da0(int param_1,undefined4 param_2)

{
  byte bVar1;
  char *pcVar2;
  byte *pbVar3;
  
  switch((int)(param_2)) {
  case 0: ;
    FUN_004b27e0(param_1,0);
    return;
  case 1: ;
    bVar1 = (&DAT_00bcd758)[param_1 * 0x184];
    pbVar3 = &DAT_00bcd758 + param_1 * 0x184;
    if (2 < bVar1) {
      FUN_004b27e0(param_1,1);
      *pbVar3 = 0;
      return;
    }
    goto LAB_004a60e3;
  case 2: ;
    if (1 < (byte)(&DAT_00bcd758)[param_1 * 0x184]) {
      (&DAT_00bcd758)[param_1 * 0x184] = '\0';
      FUN_004b27e0(param_1,1);
      return;
    }
    (&DAT_00bcd758)[param_1 * 0x184] = (&DAT_00bcd758)[param_1 * 0x184] + 1;
    return;
  case 3: ;
    pbVar3 = &DAT_00bcd758 + param_1 * 0x184;
    if ((&DAT_00bcd758)[param_1 * 0x184] == '\0') {
      FUN_004b27e0(param_1,1);
      *pbVar3 = 1;
      return;
    }
    goto LAB_004a60f4;
  case 4: ;
    pbVar3 = &DAT_00bcd758 + param_1 * 0x184;
    if ((byte)(&DAT_00bcd758)[param_1 * 0x184] < 2) {
      FUN_004b27e0(param_1,1);
      *pbVar3 = *pbVar3 + 1;
      return;
    }
    goto LAB_004a60f4;
  case 5: ;
    pbVar3 = &DAT_00bcd758 + param_1 * 0x184;
    if ((byte)(&DAT_00bcd758)[param_1 * 0x184] < 3) {
      FUN_004b27e0(param_1,1);
      *pbVar3 = *pbVar3 + 1;
      return;
    }
    goto LAB_004a60f4;
  case 6: ;
    FUN_004b27e0(param_1,1);
    return;
  case 7: ;
    pbVar3 = &DAT_00bcd758 + param_1 * 0x184;
    if ((byte)(&DAT_00bcd758)[param_1 * 0x184] < 3) {
      FUN_004b27e0(param_1,1);
      *pbVar3 = *pbVar3 + 1;
      return;
    }
    break;
  case 8: ;
    pbVar3 = &DAT_00bcd758 + param_1 * 0x184;
    if ((byte)(&DAT_00bcd758)[param_1 * 0x184] < 2) {
      FUN_004b27e0(param_1,1);
      *pbVar3 = *pbVar3 + 1;
      return;
    }
    break;
  case 9: ;
    pbVar3 = &DAT_00bcd758 + param_1 * 0x184;
    if ((&DAT_00bcd758)[param_1 * 0x184] == '\0') {
      FUN_004b27e0(param_1,1);
      *pbVar3 = *pbVar3 + 1;
      return;
    }
    break;
  case 10: ;
    pcVar2 = &DAT_00bcd758 + param_1 * 0x184;
    if ((byte)(&DAT_00bcd758)[param_1 * 0x184] < 2) {
      FUN_004b27e0(param_1,2);
      *pcVar2 = *pcVar2 + '\x01';
      return;
    }
    goto LAB_004a5faf;
  case 0xb: ;
    pcVar2 = &DAT_00bcd758 + param_1 * 0x184;
    if ((byte)(&DAT_00bcd758)[param_1 * 0x184] < 3) {
      FUN_004b27e0(param_1,2);
      *pcVar2 = *pcVar2 + '\x01';
      return;
    }
LAB_004a5faf: ;
    FUN_004b27e0(param_1,1);
    *pcVar2 = '\0';
    return;
  case 0xc: ;
    FUN_004b27e0(param_1,2);
    return;
  case 0xd: ;
    pcVar2 = &DAT_00bcd758 + param_1 * 0x184;
    if ((byte)(&DAT_00bcd758)[param_1 * 0x184] < 3) {
      FUN_004b27e0(param_1,2);
      *pcVar2 = *pcVar2 + '\x01';
      return;
    }
    goto LAB_004a6066;
  case 0xe: ;
    pcVar2 = &DAT_00bcd758 + param_1 * 0x184;
    if ((byte)(&DAT_00bcd758)[param_1 * 0x184] < 2) {
      FUN_004b27e0(param_1,2);
      *pcVar2 = *pcVar2 + '\x01';
      return;
    }
LAB_004a6066: ;
    FUN_004b27e0(param_1,3);
    *pcVar2 = '\0';
    return;
  case 0xf: ;
    pbVar3 = &DAT_00bcd758 + param_1 * 0x184;
    if ((&DAT_00bcd758)[param_1 * 0x184] == '\0') {
LAB_004a60d6: ;
      FUN_004b27e0(param_1,3);
      bVar1 = *pbVar3;
LAB_004a60e3: ;
      *pbVar3 = bVar1 + 1;
      return;
    }
    break;
  case 0x10: ;
    pbVar3 = &DAT_00bcd758 + param_1 * 0x184;
    if ((byte)(&DAT_00bcd758)[param_1 * 0x184] < 2) goto LAB_004a60d6;
    break;
  case 0x11: ;
    pbVar3 = &DAT_00bcd758 + param_1 * 0x184;
    if ((byte)(&DAT_00bcd758)[param_1 * 0x184] < 3) goto LAB_004a60d6;
    break;
default: ;
    FUN_004b27e0(param_1,3);
    return;
  }
  FUN_004b27e0(param_1,2);
LAB_004a60f4: ;
  *pbVar3 = 0;
  return;
}
