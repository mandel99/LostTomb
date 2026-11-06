#include "restore_types.h"
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)

void FUN_004b8aa0(void)

{
  uint uVar1;
  byte *pbVar2;
  int iVar3;
  
  uVar1 = DAT_00eaac28;
  pbVar2 = (byte *)0x00bcd78bu;
  iVar3 = DAT_00eaac28 * 0x49d8;
  *(undefined1 *)(0x00ea5ed9u + (uint)iVar3) = 0;
  *(undefined1 *)(0x00ea5ed8u + (uint)iVar3) = 0;
  *(undefined1 *)(0x00ea5ed4u + (uint)iVar3) = 0;
  do {
    if ((pbVar2[-1] != 0) && (*pbVar2 == uVar1)) {
      if (pbVar2[-5] == 0x4e) {
        *(undefined1 *)(0x00ea5ed8u + (uint)iVar3) =
            *(undefined1 *)(0x00ea5ed8u + (uint)iVar3) + 1;
        if (pbVar2[-0xd] != 0) {
          *(undefined1 *)(0x00ea5ed9u + (uint)iVar3) =
              *(undefined1 *)(0x00ea5ed9u + (uint)iVar3) + 1;
        }
      }
      else if (pbVar2[-5] == 0x4d) {
        *(undefined1 *)(0x00ea5ed4u + (uint)iVar3) =
            *(undefined1 *)(0x00ea5ed4u + (uint)iVar3) + 1;
      }
    }
    pbVar2 = pbVar2 + 0x184;
  } while ((int)pbVar2 < 0xc8ad47);
  return;
}
