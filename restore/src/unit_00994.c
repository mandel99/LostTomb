#include "restore_types.h"
#define DAT_0070ce10 (*(undefined4 *)0x0070ce10u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799ae5 (*(undefined4 *)0x00799ae5u)
#define DAT_00ea5f18 (*(undefined4 *)0x00ea5f18u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0046e7c0(void)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = 0;
  iVar4 = 0;
  if (0 < DAT_0078d790) {
    pcVar2 = &DAT_00799ae5;
    iVar3 = DAT_0078d790;
    do {
      if ((pcVar2[-5] != '\0') && (*pcVar2 != '\0')) {
        iVar1 = iVar1 + *(int *)(&DAT_0070ce10 + *(short *)(pcVar2 + 0xd) * 0x58) *
                        (int)*(short *)(pcVar2 + 0x17);
        iVar4 = iVar4 + *(short *)(pcVar2 + 0x17);
      }
      pcVar2 = pcVar2 + 0x108;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    if (0 < iVar4) {
      (&DAT_00ea5f18)[DAT_00eaac28 * 0x1276] = iVar1 / iVar4;
      return;
    }
  }
  (&DAT_00ea5f18)[DAT_00eaac28 * 0x1276] = 0;
  return;
}
