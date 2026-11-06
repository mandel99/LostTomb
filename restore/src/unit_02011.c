#include "restore_types.h"
#define DAT_0078d790 (*(int32_t *)0x0078d790u)
#define DAT_0078d7ac (*(undefined *)0x0078d7acu)
#define DAT_0078d7b0 (*(undefined *)0x0078d7b0u)
#define DAT_00799bc8 (*(undefined4 *)0x00799bc8u)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
extern void FUN_004b3250();
extern void FUN_004b5420();
extern void FUN_004e8e80();

void FUN_004e7200(int param_1)

{
  int *piVar1;
  int iVar2;
  char *pcVar3;
  
  if (param_1 < 3) {
    param_1 = 3;
  }
  else if (9 < param_1) {
    param_1 = 9;
  }
  if (0 < DAT_0078d790) {
    piVar1 = &DAT_00799bc8;
    iVar2 = DAT_0078d790;
    do {
      if (*(char *)((int)piVar1 + -0xe3) != '\0') {
        *piVar1 = *piVar1 + (int)(5000 / (longlong)(piVar1[3] + 1));
        piVar1[5] = piVar1[5] + (int)(5000 / (longlong)(piVar1[3] + 1));
        if (piVar1[3] * 3 < (int)(uint)*(byte *)((int)piVar1 + -0x71)) {
          *(char *)((int)piVar1 + -0x71) = (char)(piVar1[3] * 3);
        }
      }
      piVar1 = piVar1 + 0x42;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  }
  iVar2 = 1;
  pcVar3 = &DAT_00bcd786;
  do {
    if (pcVar3[4] != '\0') {
      if (*pcVar3 == '_') {
        FUN_004b5420(iVar2);
      }
      else if ((*pcVar3 == '\x19') && ((pcVar3[0x2e] == '\n' || (pcVar3[0x2e] == '\t')))) {
        pcVar3[0x2c] = '\0';
        pcVar3[0x2d] = '\0';
        pcVar3[0x2e] = '\r';
        *(undefined2 *)(pcVar3 + 0x1a) = *(undefined2 *)(pcVar3 + 0x22);
        *(undefined2 *)(pcVar3 + 0x1c) = *(undefined2 *)(pcVar3 + 0x24);
        FUN_004b3250(iVar2);
      }
    }
    pcVar3 = pcVar3 + 0x184;
    iVar2 = iVar2 + 1;
  } while ((int)pcVar3 < 0xc8ad42);
  DAT_0078d7ac = param_1 * 0x330;
  DAT_0078d7b0 = DAT_0078d7ac;
  FUN_004e8e80(0xfffffff1);
  return;
}
