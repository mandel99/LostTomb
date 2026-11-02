#include "restore_types.h"
#define DAT_009784dc (*(undefined4 *)0x009784dcu)
#define DAT_00eaaf68 (*(undefined4 *)0x00eaaf68u)
extern undefined4 FUN_004f6570();

undefined4 FUN_004b47a0(int param_1)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
  piVar3 = &DAT_00eaaf68;
  while ((*piVar3 == 0 || (iVar1 = FUN_004f6570(param_1), iVar1 == 0))) {
    piVar3 = piVar3 + 0x38;
    if (0xeadb27 < (int)piVar3) {
      piVar3 = &DAT_009784dc;
      do {
        if ((char)piVar3[-1] != '\0') {
          iVar1 = 0;
          piVar2 = piVar3;
          do {
            if (*piVar2 == param_1) {
              return 1;
            }
            iVar1 = iVar1 + 1;
            piVar2 = piVar2 + 1;
          } while (iVar1 < 4);
        }
        piVar3 = piVar3 + 0xb;
      } while ((int)piVar3 < 0x978d74);
      return 0;
    }
  }
  return 1;
}
