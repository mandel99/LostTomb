#include "restore_types.h"
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)

int FUN_0040f070(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  char cVar1;
  int *in_ECX;
  int iVar2;
  short *psVar3;
  int iVar4;
  
  iVar2 = 0;
  iVar4 = 1;
  psVar3 = &DAT_00799af0;
  do {
    if (iVar2 != 0) {
      return iVar2;
    }
    if ((((char)psVar3[-8] == '\x01') && (*psVar3 == 0xe1)) &&
       (cVar1 = (**(code **)(*in_ECX + 0x2c))(iVar4,param_3,1), cVar1 != '\0')) {
      iVar2 = iVar4;
    }
    psVar3 = psVar3 + 0x84;
    iVar4 = iVar4 + 1;
  } while ((int)psVar3 < 0x89b6e8);
  return iVar2;
}
