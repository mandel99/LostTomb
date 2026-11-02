#include "restore_types.h"
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
extern undefined4 FUN_004ea9a0();

int FUN_004eaa60(int param_1,uint param_2)

{
  char cVar1;
  int iVar2;
  undefined1 *puVar3;
  
  iVar2 = 0;
  puVar3 = &DAT_00bcd786;
  do {
    if (puVar3[4] != '\0') {
      cVar1 = FUN_004ea9a0(*puVar3);
      if (((cVar1 != '\0') && (*(short *)(puVar3 + 0x56) == param_1)) &&
         ((byte)puVar3[0x2e] == param_2)) {
        iVar2 = iVar2 + 1;
      }
    }
    puVar3 = puVar3 + 0x184;
  } while ((int)puVar3 < 0xc8ad42);
  return iVar2;
}
