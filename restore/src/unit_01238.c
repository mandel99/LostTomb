#include "restore_types.h"
#define DAT_00bc97c0 (*(int32_t *)0x00bc97c0u)

int FUN_00484f40(void)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = 0;
  piVar2 = &DAT_00bc97c0;
  do {
    if (*piVar2 != 0) {
      iVar1 = iVar1 + 1;
    }
    piVar2 = piVar2 + 1;
  } while ((int)piVar2 < 0xbc97e0);
  return iVar1;
}
