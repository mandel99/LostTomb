#include "restore_types.h"
#include "town_menu.h"
#define DAT_005d2d40 (*(undefined4 *)0x005d2d40u)
#define DAT_005d2d44 (*(undefined4 *)0x005d2d44u)
#define DAT_005d2e60 (*(undefined4 *)0x005d2e60u)
#define DAT_005d2e64 (*(undefined4 *)0x005d2e64u)
#define DAT_00785d70 (*(int16_t *)0x00785d70u)
#define DAT_00785d7c (*(undefined4 *)0x00785d7cu)

void FUN_004d40c0(void)

{
  char cVar1;
  short sVar2;
  int iVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short *psVar7;
  
  iVar3 = (int)(short)DAT_00785d70;
  if (0 < iVar3) {
    psVar7 = (short *)&DAT_00785d7c;
    do {
      cVar1 = FUN_004493c0((const RestoreTownRecord *)(psVar7-4));
      if (cVar1 != '\0') {
        sVar5 = psVar7[-2];
        sVar2 = psVar7[-1];
        sVar6 = *psVar7;
        sVar4 = psVar7[1];
        if (0x1e < sVar5) {
          sVar5 = sVar5 + 5;
        }
        psVar7[-2] = sVar5;
        if (0x1e < sVar2) {
          sVar2 = sVar2 + 5;
        }
        if (0x1e < sVar6) {
          sVar6 = sVar6 + 5;
        }
        if (0x1e < sVar4) {
          sVar4 = sVar4 + 5;
        }
        sVar5 = sVar4;
        if (sVar2 < 0) {
          if (sVar6 < 0) {
            if (-1 < sVar4) {
              sVar5 = -1;
              sVar2 = sVar4;
            }
            goto LAB_004d4139;
          }
          sVar2 = sVar6;
          sVar6 = -1;
LAB_004d413e: ;
          sVar5 = sVar4;
          if (-1 < sVar4) {
            sVar5 = -1;
            sVar6 = sVar4;
          }
        }
        else {
LAB_004d4139: ;
          sVar4 = sVar5;
          if (sVar6 < 0) goto LAB_004d413e;
        }
        psVar7[-1] = sVar2;
        *psVar7 = sVar6;
        psVar7[1] = sVar5;
      }
      psVar7 = psVar7 + 0x3e;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  iVar3 = 0;
  do {
    *(undefined4 *)((int)&DAT_005d2d40 + iVar3) = *(undefined4 *)((int)&DAT_005d2e60 + iVar3);
    *(undefined4 *)((int)&DAT_005d2d44 + iVar3) = *(undefined4 *)((int)&DAT_005d2e64 + iVar3);
    iVar3 = iVar3 + 8;
  } while (iVar3 < 0x28);
  return;
}
