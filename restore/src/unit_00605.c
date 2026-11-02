#include "restore_types.h"
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_0077f749 (*(undefined4 *)0x0077f749u)
extern void FUN_0040f820();
extern void FUN_0040f8f0();

void FUN_00442540(int param_1)

{
  undefined1 *puVar1;
  
  if ((&DAT_0077f749)[param_1 * 0x62] != '\0') {
    FUN_0040f8f0(&DAT_0077f748 + param_1 * 0x62,0x62,0);
    if (param_1 < 199) {
      puVar1 = &DAT_0077f748 + (param_1 + 1) * 0x62;
      do {
        if (puVar1[1] == '\0') {
          return;
        }
        FUN_0040f820(puVar1,puVar1 + -0x62,0x62);
        FUN_0040f8f0(puVar1,0x62,0);
        puVar1 = puVar1 + 0x62;
        param_1 = param_1 + 1;
      } while (param_1 < 199);
    }
  }
  return;
}
