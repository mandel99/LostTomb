#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799b98 (*(undefined4 *)0x00799b98u)
extern undefined4 FUN_0045a140();

void FUN_004684e0(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  
  iVar3 = 1;
  if (0 < DAT_0078d790) {
    puVar2 = &DAT_00799b98;
    do {
      if ((*(char *)(puVar2 + -0x2e) == '\x01') &&
         (*puVar2 = 0, *(char *)((int)puVar2 + -0xb3) == '\0')) {
        uVar1 = FUN_0045a140(iVar3);
        *puVar2 = uVar1;
      }
      iVar3 = iVar3 + 1;
      puVar2 = puVar2 + 0x42;
    } while (iVar3 <= DAT_0078d790);
  }
  return;
}
