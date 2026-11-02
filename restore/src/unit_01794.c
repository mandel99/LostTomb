#include "restore_types.h"
#define DAT_0077f760 (*(undefined4 *)0x0077f760u)

void FUN_004d17e0(void)

{
  undefined1 *puVar1;
  
  puVar1 = &DAT_0077f760;
  do {
    if ((puVar1[-0x17] != '\0') && (puVar1[-0x18] == '\x01')) {
      switch(*puVar1) {
      case 0: ;
      case 3: ;
      case 5: ;
        *puVar1 = 4;
        break;
      case 1: ;
        *puVar1 = 0;
        break;
      case 2: ;
        *puVar1 = 1;
        break;
      case 4: ;
      case 6: ;
        *puVar1 = 6;
      }
    }
    puVar1 = puVar1 + 0x62;
  } while ((int)puVar1 < 0x7843f0);
  return;
}
