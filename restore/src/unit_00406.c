#include "restore_types.h"
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)

void FUN_00425b80(int param_1,int param_2)

{
  char *pcVar1;
  char cVar2;
  
  if (0 < param_2) {
    pcVar1 = (char *)(param_1 + 0x19);
    do {
      if ((*pcVar1 != '\0') &&
         (((*(short *)(pcVar1 + -0x11) == 4 || (*(short *)(pcVar1 + -0x11) == 6)) &&
          (cVar2 = *pcVar1 + -1, *pcVar1 = cVar2, cVar2 == '\0')))) {
        *pcVar1 = '\0';
        DAT_00e38dc9 = 1;
      }
      pcVar1 = pcVar1 + 0x24;
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return;
}
