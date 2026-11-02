#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799bcc (*(undefined4 *)0x00799bccu)
extern void FUN_0042fb30();

void FUN_00430250(void)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  iVar3 = 1;
  if (0 < DAT_0078d790) {
    piVar2 = &DAT_00799bcc;
    do {
      if (((((char)piVar2[-0x3b] != '\0') && (*(char *)((int)piVar2 + -0xe7) != '\0')) &&
          ((short)piVar2[-0x34] != 0)) &&
         ((*piVar2 != 0 && (iVar1 = *piVar2 + -1, *piVar2 = iVar1, iVar1 == 0)))) {
        FUN_0042fb30(iVar3);
      }
      iVar3 = iVar3 + 1;
      piVar2 = piVar2 + 0x42;
    } while (iVar3 <= DAT_0078d790);
  }
  return;
}
