#include "restore_types.h"
#define DAT_00bcba9a (*(undefined4 *)0x00bcba9au)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
extern int FUN_004b8330();

int FUN_004b8360(int param_1,int *param_2,int *param_3)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  int iVar6;
  
  if (param_1 < 0) {
    return 0;
  }
  iVar6 = 0;
  pcVar5 = &DAT_00bcba9a;
  iVar4 = 1;
  do {
    if ((((pcVar5[-0x42] == '\x01') && (*pcVar5 != '\0')) && (pcVar5[0x25] != '\0')) &&
       ((bVar1 = pcVar5[-0x1a], bVar1 != 0 && (*(short *)(pcVar5 + 0x40) == param_1)))) {
      if (*(short *)(pcVar5 + -0x3e) == 0xd) {
        iVar3 = (*(short *)(pcVar5 + 0x38) + 200) * (uint)bVar1;
      }
      else {
        iVar3 = (*(short *)(pcVar5 + 0x38) + 100) * (uint)bVar1;
      }
      iVar6 = iVar6 + iVar3 / 100;
    }
    pcVar5 = pcVar5 + 0x90;
  } while ((int)pcVar5 < 0xbcd62a);
  if (param_3 != (int *)0x0) {
    *param_3 = iVar6;
  }
  pcVar5 = &DAT_00bcd786;
  iVar3 = iVar6;
  do {
    if (((pcVar5[4] != '\0') && (*pcVar5 == 'N')) &&
       (((byte)pcVar5[5] == DAT_00eaac28 && (*(short *)(pcVar5 + 0x128) == param_1)))) {
      iVar2 = FUN_004b8330(iVar4);
      iVar3 = iVar3 + iVar2;
    }
    pcVar5 = pcVar5 + 0x184;
    iVar4 = iVar4 + 1;
  } while ((int)pcVar5 < 0xc8ad42);
  if (param_2 != (int *)0x0) {
    *param_2 = iVar3 - iVar6;
  }
  return iVar3;
}
