#include "restore_types.h"
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_010d6a0c (*(undefined4 *)0x010d6a0cu)
#define DAT_010d6a0e (*(undefined4 *)0x010d6a0eu)
#define DAT_0126517c (*(undefined *)0x0126517cu)

undefined4 FUN_00407f90(void)

{
  byte bVar1;
  short sVar2;
  short sVar3;
  uint uVar4;
  int iVar5;
  int in_ECX;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  short sVar10;
  char *pcVar11;
  uint local_14;
  
  *(undefined2 *)(in_ECX + 0x13e) = 0xffff;
  *(undefined2 *)(in_ECX + 0x13c) = 0xffff;
  bVar1 = *(byte *)(in_ECX + 0x141);
  iVar5 = CONCAT31(0xffffff,bVar1);
  local_14 = (uint)bVar1;
  if (bVar1 == 0) {
    return 0xffffff00;
  }
  sVar10 = 0;
  pcVar11 = &DAT_0077f748;
  do {
    iVar5 = CONCAT31((int3)((uint)iVar5 >> 8),pcVar11[1]);
    if ((pcVar11[1] != '\0') && (*pcVar11 == '\x01')) {
      uVar4 = (uint)*(ushort *)(pcVar11 + 0xc);
      uVar6 = uVar4;
      if (DAT_0126517c <= (int)uVar4) {
        uVar6 = 0;
      }
      sVar2 = (&DAT_010d6a0c)[uVar6 * 0x20];
      if (DAT_0126517c <= (int)uVar4) {
        uVar4 = 0;
      }
      iVar9 = (int)*(short *)(pcVar11 + 6);
      iVar7 = (int)*(short *)(pcVar11 + 4);
      sVar3 = (&DAT_010d6a0e)[uVar4 * 0x20];
      if ((((iVar7 <= *(short *)(in_ECX + 8)) && ((int)*(short *)(in_ECX + 8) <= sVar2 + iVar7)) &&
          (iVar9 <= *(short *)(in_ECX + 10))) && ((int)*(short *)(in_ECX + 10) <= sVar3 + iVar9)) {
        *(short *)(in_ECX + 0x13c) = sVar10;
      }
      iVar8 = (int)*(short *)(in_ECX + 2 + local_14 * 6);
      iVar5 = in_ECX + local_14 * 6;
      if (((iVar7 <= iVar8) && (iVar8 <= sVar2 + iVar7)) &&
         ((iVar5 = (int)*(short *)(iVar5 + 4), iVar9 <= iVar5 && (iVar5 <= sVar3 + iVar9)))) {
        *(short *)(in_ECX + 0x13e) = sVar10;
      }
    }
    pcVar11 = pcVar11 + 0x62;
    sVar10 = sVar10 + 1;
  } while ((int)pcVar11 < 0x7843d8);
  return CONCAT31((int3)((uint)iVar5 >> 8),1);
}
