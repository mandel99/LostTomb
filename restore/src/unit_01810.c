#include "restore_types.h"
#define DAT_00bcd78a (*(undefined4 *)0x00bcd78au)

void FUN_004d3cc0(void)

{
  char *pcVar1;
  
  pcVar1 = &DAT_00bcd78a;
  do {
    if (((pcVar1[-4] == 'N') && (*pcVar1 == '\x01')) && (pcVar1[0x2a] != '\x04')) {
      *(undefined2 *)(pcVar1 + 0x13e) = *(undefined2 *)(pcVar1 + 6);
      *(undefined2 *)(pcVar1 + 0x140) = *(undefined2 *)(pcVar1 + 8);
    }
    pcVar1 = pcVar1 + 0x184;
  } while ((int)pcVar1 < 0xc8ad46);
  return;
}
