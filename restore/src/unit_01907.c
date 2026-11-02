#include "restore_types.h"
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
extern uint FUN_004e05f0();

int FUN_004e0c00(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  char cVar1;
  short *psVar2;
  int iVar3;
  
  iVar3 = 1;
  psVar2 = &DAT_00799af0;
  do {
    if (((char)psVar2[-8] == '\x01') && (*psVar2 == 0xde)) {
      cVar1 = FUN_004e05f0(iVar3,param_3,1);
      if (cVar1 != '\0') {
        return iVar3;
      }
    }
    psVar2 = psVar2 + 0x84;
    iVar3 = iVar3 + 1;
  } while ((int)psVar2 < 0x89b6e8);
  return 0;
}
