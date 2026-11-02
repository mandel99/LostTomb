#include "restore_types.h"
#define DAT_006043e6 (*(undefined4 *)0x006043e6u)

void FUN_0055d150(int param_1)

{
  byte *pbVar1;
  uint uVar2;
  byte bVar3;
  undefined1 uVar4;
  int iVar5;
  ushort *puVar6;
  
  iVar5 = 0xff;
  puVar6 = (ushort *)0x006043e6u;
  do {
    pbVar1 = (byte *)(param_1 + 0x2fb4 + iVar5);
    bVar3 = *pbVar1;
    uVar4 = (undefined1)iVar5;
    if (bVar3 < 9) {
      uVar2 = (uint)*puVar6;
      do {
        *(undefined1 *)(param_1 + 0x2c34 + uVar2) = uVar4;
        uVar2 = uVar2 + (1 << (bVar3 & 0x1f));
      } while (uVar2 < 0x100);
    }
    else if ((*puVar6 & 0xff) == 0) {
      *pbVar1 = bVar3 - 8;
      uVar2 = (uint)(*puVar6 >> 8);
      do {
        *(undefined1 *)(param_1 + 0x2eb4 + uVar2) = uVar4;
        uVar2 = uVar2 + (1 << (bVar3 - 8 & 0x1f));
      } while (uVar2 < 0x100);
    }
    else {
      *(undefined1 *)((*puVar6 & 0xff) + 0x2c34 + param_1) = 0xff;
      if ((*puVar6 & 0x3f) == 0) {
        bVar3 = *pbVar1 - 6;
        *pbVar1 = bVar3;
        uVar2 = (uint)(*puVar6 >> 6);
        do {
          *(undefined1 *)(param_1 + 0x2e34 + uVar2) = uVar4;
          uVar2 = uVar2 + (1 << (bVar3 & 0x1f));
        } while (uVar2 < 0x80);
      }
      else {
        bVar3 = *pbVar1 - 4;
        *pbVar1 = bVar3;
        uVar2 = (uint)(*puVar6 >> 4);
        do {
          *(undefined1 *)(param_1 + 0x2d34 + uVar2) = uVar4;
          uVar2 = uVar2 + (1 << (bVar3 & 0x1f));
        } while (uVar2 < 0x100);
      }
    }
    puVar6 = puVar6 + -1;
    iVar5 = iVar5 + -1;
  } while ((ushort *)0x6041e7 < puVar6);
  return;
}
