#include "restore_types.h"
#define DAT_00799bc4 (*(undefined4 *)0x00799bc4u)
#define DAT_00ea1880 (*(undefined4 *)0x00ea1880u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_004c5120();
extern bool FUN_004c52d0();

void FUN_004e8db0(void)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  
  iVar1 = *(int *)(&DAT_00ea1880 + DAT_00eaac28 * 0x49d8);
  iVar5 = 0;
  if ((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) {
    iVar3 = FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]);
    if (iVar3 == 1) {
      cVar2 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],1);
      if (cVar2 != '\0') {
        iVar5 = -2;
      }
    }
  }
  piVar4 = &DAT_00799bc4;
  do {
    if ((((char)piVar4[-0x39] == '\x01') && (*(char *)((int)piVar4 + -0xdf) != '\0')) &&
       ((short)piVar4[-0x32] != 0)) {
      iVar3 = *piVar4 + iVar5 + (0x50 - iVar1) / 5;
      *piVar4 = iVar3;
      if (iVar3 < 0) {
        *piVar4 = 0;
      }
      if (1000 < *piVar4) {
        *piVar4 = 1000;
      }
    }
    piVar4 = piVar4 + 0x42;
  } while ((int)piVar4 < 0x89b7bc);
  return;
}
