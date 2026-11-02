#include "restore_types.h"
#define DAT_00777236 (*(undefined4 *)0x00777236u)

int FUN_00407340(void)

{
  int iVar1;
  char *pcVar2;
  
  iVar1 = 1;
  pcVar2 = &DAT_00777236;
  while( true ) {
    if ((byte)pcVar2[-1] < 2) {
      *pcVar2 = '\0';
    }
    if (*pcVar2 == '\0') break;
    pcVar2 = pcVar2 + 0x144;
    iVar1 = iVar1 + 1;
    if (0x778a41 < (int)pcVar2) {
      return -1;
    }
  }
  return iVar1;
}
