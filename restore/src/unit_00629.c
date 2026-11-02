#include "restore_types.h"
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
extern undefined4 FUN_00443040();

undefined4 FUN_00443ad0(undefined4 param_1)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  
  iVar3 = 0;
  pcVar2 = &DAT_0077f748;
  do {
    if ((pcVar2[1] != '\0') && (*pcVar2 == '\x01')) {
      iVar1 = FUN_00443040(iVar3,param_1);
      if (iVar1 != 0) {
        return 1;
      }
    }
    pcVar2 = pcVar2 + 0x62;
    iVar3 = iVar3 + 1;
  } while ((int)pcVar2 < 0x7843d8);
  return 0;
}
