#include "restore_types.h"
#define DAT_00799b94 (*(undefined4 *)0x00799b94u)
extern undefined4 FUN_00459240();
extern int FUN_00488030();
extern uint FUN_004894a0();

uint FUN_004bf9d0(void)

{
  uint uVar1;
  int in_ECX;
  short *psVar2;
  int iVar3;
  
  uVar1 = *(uint *)(in_ECX + 0x32fb4);
  if (((uVar1 != 1) && (uVar1 != 2)) && (uVar1 != 3)) {
    iVar3 = 1;
    psVar2 = &DAT_00799b94;
    do {
      if ((char)psVar2[-0x5a] == '\x01') {
        uVar1 = FUN_00488030(iVar3);
        if ((char)uVar1 != '\0') {
          uVar1 = FUN_004894a0(iVar3);
          if (((char)uVar1 != '\0') && (*psVar2 == 0)) {
            uVar1 = FUN_00459240(iVar3);
            if (uVar1 == 0) {
              return 1;
            }
          }
        }
      }
      psVar2 = psVar2 + 0x84;
      iVar3 = iVar3 + 1;
    } while ((int)psVar2 < 0x89b78c);
  }
  return uVar1 & 0xffffff00;
}
