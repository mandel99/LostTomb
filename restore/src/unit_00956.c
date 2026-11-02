#include "restore_types.h"
#define DAT_00705ea8 (*(undefined4 *)0x00705ea8u)
#define DAT_00705eac (*(undefined4 *)0x00705eacu)
#define DAT_00705eb0 (*(undefined4 *)0x00705eb0u)
#define DAT_00705eb4 (*(undefined4 *)0x00705eb4u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)
extern void FUN_0040f8f0();
extern void FUN_00481830();

void FUN_0046a0c0(void)

{
  int iVar1;
  short *psVar2;
  int iVar3;
  
  FUN_0040f8f0(&DAT_00ac2d4c,0xcb10,0);
  iVar3 = 1;
  if (0 < DAT_0078d790) {
    psVar2 = &DAT_00799af0;
    do {
      if ((char)psVar2[-8] == '\x01') {
        iVar1 = *psVar2 * 0x28;
        FUN_00481830((int)psVar2[-5],(int)psVar2[-4],*(undefined1 *)((int)psVar2 + -0xd),
                     *(undefined4 *)(&DAT_00705ea8 + iVar1),*(undefined4 *)(&DAT_00705eac + iVar1),
                     *(undefined4 *)(&DAT_00705eb0 + iVar1),*(undefined4 *)(&DAT_00705eb4 + iVar1));
      }
      iVar3 = iVar3 + 1;
      psVar2 = psVar2 + 0x84;
    } while (iVar3 <= DAT_0078d790);
  }
  return;
}
