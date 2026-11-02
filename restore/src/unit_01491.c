#include "restore_types.h"
#define DAT_00bcba9a (*(undefined4 *)0x00bcba9au)

void FUN_004b8080(void)

{
  char *pcVar1;
  
  pcVar1 = &DAT_00bcba9a;
  do {
    if ((pcVar1[-0x42] == '\x01') && (*pcVar1 != '\0')) {
      pcVar1[-0x19] = '\x10';
    }
    pcVar1 = pcVar1 + 0x90;
  } while ((int)pcVar1 < 0xbcd62a);
  return;
}
