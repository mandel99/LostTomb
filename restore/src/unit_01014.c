#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
extern undefined4 FUN_0046f930();

void FUN_0046fc30(void)

{
  char cVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  
  iVar4 = 0;
  puVar2 = &DAT_00af598c;
  do {
    iVar5 = 0;
    puVar3 = puVar2 + 0xe4;
    do {
      cVar1 = FUN_0046f930(iVar5,iVar4);
      if (cVar1 == '\0') {
        *puVar2 = 0x80005;
      }
      iVar5 = iVar5 + 1;
      puVar2 = puVar2 + 1;
    } while (iVar5 < 0xe4);
    iVar4 = iVar4 + 1;
    puVar2 = puVar3;
  } while (iVar4 < 0xe4);
  return;
}
