#include "restore_types.h"
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_00f56a0c (*(undefined *)0x00f56a0cu)
#define DAT_00f56a14 (*(undefined *)0x00f56a14u)
extern int FUN_004425c0();

void FUN_00442660(void)

{
  undefined2 uVar1;
  ushort uVar2;
  int iVar3;
  char *pcVar4;
  int iVar5;
  
  iVar5 = 0;
  pcVar4 = &DAT_0077f748;
  iVar3 = DAT_00f56a14;
  do {
    if (pcVar4[1] != '\0') {
      if (*pcVar4 == '\x01') {
        uVar1 = FUN_004425c0(iVar5);
        iVar3 = DAT_00f56a14;
        *(undefined2 *)(pcVar4 + 0xc) = uVar1;
      }
      else if ((*pcVar4 == '\0') &&
              (uVar2 = *(short *)(pcVar4 + 0xe) + (short)DAT_00f56a0c,
              *(ushort *)(pcVar4 + 0xc) = uVar2, iVar3 < (int)(uint)uVar2)) {
        *(short *)(pcVar4 + 0xc) = (short)iVar3;
      }
    }
    pcVar4 = pcVar4 + 0x62;
    iVar5 = iVar5 + 1;
  } while ((int)pcVar4 < 0x7843d8);
  return;
}
