#include "restore_types.h"
#define PTR_DAT_005dbbd8 (*(unsigned char * *)0x005dbbd8u)

void FUN_0046f8d0(void)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined **ppuVar5;
  undefined1 *puVar6;
  int iVar7;
  
  iVar7 = -1;
  ppuVar5 = &PTR_DAT_005dbbd8;
  iVar4 = 1;
  do {
    puVar6 = *ppuVar5;
    for (iVar3 = iVar7; iVar2 = iVar7, iVar3 <= iVar4; iVar3 = iVar3 + 1) {
      for (; iVar2 <= iVar4; iVar2 = iVar2 + 1) {
        if ((((iVar2 == iVar7) || (iVar2 == iVar4)) || (iVar3 == iVar7)) || (iVar3 == iVar4)) {
          *puVar6 = (char)iVar2;
          puVar6[1] = (char)iVar3;
          puVar6 = puVar6 + 2;
        }
      }
    }
    iVar7 = iVar7 + -1;
    ppuVar5 = ppuVar5 + 1;
    bVar1 = iVar4 < 0xc;
    iVar4 = iVar4 + 1;
  } while (bVar1);
  return;
}
