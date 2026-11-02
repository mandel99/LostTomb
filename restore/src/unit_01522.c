#include "restore_types.h"
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
extern undefined4 FUN_00559110();

int FUN_004bc2e0(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  
  iVar3 = 0;
  iVar4 = 1;
  psVar5 = &DAT_00799af0;
  do {
    if (iVar3 != 0) {
      return iVar3;
    }
    if ((char)psVar5[-8] == '\x01') {
      sVar1 = *psVar5;
      if (sVar1 == 0xe5) {
        if (*(int *)(psVar5 + 0x28) == 2) {
          cVar2 = FUN_00559110(iVar4,param_3,1);
          goto joined_r0x004bc385;
        }
      }
      else if (sVar1 == 0xea) {
        if (*(int *)(psVar5 + 0x28) == 2) {
          cVar2 = FUN_00559110(iVar4,param_3,1);
          goto joined_r0x004bc385;
        }
      }
      else if (sVar1 == 0xeb) {
        if (*(int *)(psVar5 + 0x28) == 2) {
          cVar2 = FUN_00559110(iVar4,param_3,1);
joined_r0x004bc385: ;
          if (cVar2 != '\0') {
            iVar3 = iVar4;
          }
        }
      }
      else if ((sVar1 == 0xec) && (*(int *)(psVar5 + 0x28) == 2)) {
        cVar2 = FUN_00559110(iVar4,param_3,1);
        goto joined_r0x004bc385;
      }
    }
    psVar5 = psVar5 + 0x84;
    iVar4 = iVar4 + 1;
    if (0x89b6e7 < (int)psVar5) {
      return iVar3;
    }
  } while( true );
}
