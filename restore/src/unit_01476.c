#include "restore_types.h"
#define DAT_00bcba9a (*(undefined4 *)0x00bcba9au)

undefined4 FUN_004b7590(void)

{
  char *pcVar1;
  
  pcVar1 = &DAT_00bcba9a;
  while (((pcVar1[-0x42] != '\x01' || (*pcVar1 == '\0')) || (*(short *)(pcVar1 + 4) < 1))) {
    pcVar1 = pcVar1 + 0x90;
    if (0xbcd629 < (int)pcVar1) {
      return 0;
    }
  }
  return 1;
}
