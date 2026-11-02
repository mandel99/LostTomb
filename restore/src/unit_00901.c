#include "restore_types.h"
#define DAT_00784bbc (*(undefined *)0x00784bbcu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00ea624a (*(undefined1 *)0x00ea624au)
extern void FUN_0045d630();

void FUN_0045d5d0(void)

{
  int iVar1;
  short *psVar2;
  int iVar3;
  
  if (DAT_00ea624a != '\0') {
    FUN_0045d630(1);
  }
  iVar1 = DAT_00784bbc;
  if (0 < DAT_0078d790) {
    psVar2 = &DAT_00799af0;
    iVar3 = DAT_0078d790;
    do {
      if (((char)psVar2[-8] == '\x01') && (((*psVar2 == 0x46 || (*psVar2 == 0xbe)) && (iVar1 != 0)))
         ) {
        psVar2[0x23] = 200;
      }
      psVar2 = psVar2 + 0x84;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  return;
}
