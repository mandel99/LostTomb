#include "restore_types.h"
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
extern uint FUN_00401d40();

int FUN_00401ed0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  
  iVar2 = 0;
  iVar3 = 1;
  psVar4 = &DAT_00799af0;
  do {
    if (iVar2 != 0) {
      return iVar2;
    }
    if ((((char)psVar4[-8] == '\x01') && (*psVar4 == 0xe6)) &&
       (cVar1 = FUN_00401d40(iVar3,param_3,1,param_4), cVar1 != '\0')) {
      iVar2 = iVar3;
    }
    psVar4 = psVar4 + 0x84;
    iVar3 = iVar3 + 1;
  } while ((int)psVar4 < 0x89b6e8);
  return iVar2;
}
