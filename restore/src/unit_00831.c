#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d83c (*(undefined *)0x0078d83cu)
#define DAT_0078e178 (*(undefined4 *)0x0078e178u)
#define DAT_00799ae5 (*(undefined4 *)0x00799ae5u)

void FUN_004583f0(void)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  undefined2 *puVar4;
  int iVar5;
  
  iVar1 = DAT_0078d790;
  iVar2 = 1;
  iVar5 = 0;
  DAT_0078d83c = 0;
  if (0 < DAT_0078d790) {
    puVar4 = &DAT_0078e178;
    pcVar3 = &DAT_00799ae5;
    do {
      if ((pcVar3[-5] == '\x01') && (*pcVar3 != '\0')) {
        *puVar4 = (short)iVar2;
        iVar5 = iVar5 + 1;
        puVar4 = puVar4 + 1;
      }
      iVar2 = iVar2 + 1;
      pcVar3 = pcVar3 + 0x108;
      DAT_0078d83c = iVar5;
    } while (iVar2 <= iVar1);
  }
  return;
}
