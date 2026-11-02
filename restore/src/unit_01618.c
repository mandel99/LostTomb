#include "restore_types.h"
#define DAT_007009dc (*(undefined4 *)0x007009dcu)
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00d386e0 (*(undefined4 *)0x00d386e0u)
#define DAT_00d38700 (*(undefined *)0x00d38700u)
#define DAT_00d38708 (*(undefined4 *)0x00d38708u)
#define DAT_00d38710 (*(undefined4 *)0x00d38710u)
#define DAT_00d38750 (*(undefined4 *)0x00d38750u)
#define DAT_00e38dd7 (*(undefined1 *)0x00e38dd7u)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define s_save_highscore_jas_005e02a8 (*(char (*)[19])0x005e02a8u)
extern int FUN_004192b0();
extern int FUN_0041c380();
extern void FUN_0041c600();
extern bool FUN_004c5880();
extern void FUN_004c5970();
extern int FUN_00562370();

void FUN_004c5a40(void)

{
  int iVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  char *pcVar8;
  char *pcVar9;
  
  DAT_00d38700 = 0;
  if (DAT_00e38ebc == 0) {
    uVar7 = DAT_007848d8 - DAT_0078d7bc >> 0x1f;
    iVar1 = DAT_0078d7c0 + ((DAT_007848d8 - DAT_0078d7bc ^ uVar7) - uVar7) * 0xc;
    iVar3 = FUN_0041c380(iVar1);
    uVar4 = FUN_004192b0();
    if (DAT_00e38dd7 != '\0') {
      DAT_00e38dd7 = 0;
      DAT_00d38700 = 1;
      return;
    }
    FUN_004c5880(s_save_highscore_jas_005e02a8);
    iVar5 = (int)DAT_00e38ea8;
    if ((&DAT_00d38750)[iVar5 * 0x13] == 0) {
      DAT_00d38700 = 2;
      FUN_004c5970(iVar5,uVar4,iVar3,iVar1);
    }
    else {
      iVar5 = FUN_00562370(&DAT_00d38710 + iVar5 * 0x4c,&DAT_00e38e74);
      if (iVar5 == 0) {
        if ((iVar3 < *(int *)(&DAT_00d38708 + DAT_00e38ea8 * 0x4c)) ||
           (iVar3 <= *(int *)(&DAT_00d38708 + DAT_00e38ea8 * 0x4c))) {
          DAT_00d38700 = 3;
        }
        else {
          DAT_00d38700 = 4;
          FUN_004c5970((int)DAT_00e38ea8,uVar4,iVar3,iVar1);
        }
      }
      else {
        iVar5 = (int)DAT_00e38ea8;
        if ((iVar3 < *(int *)(&DAT_00d38708 + iVar5 * 0x4c)) ||
           (iVar3 <= *(int *)(&DAT_00d38708 + iVar5 * 0x4c))) {
          DAT_00d38700 = 6;
        }
        else {
          DAT_00d38700 = 5;
          uVar7 = 0xffffffff;
          pcVar8 = &DAT_00d38710 + iVar5 * 0x4c;
          do {
            pcVar9 = pcVar8;
            if (uVar7 == 0) break;
            uVar7 = uVar7 - 1;
            pcVar9 = pcVar8 + 1;
            cVar2 = *pcVar8;
            pcVar8 = pcVar9;
          } while (cVar2 != '\0');
          uVar7 = ~uVar7;
          pcVar8 = pcVar9 + -uVar7;
          pcVar9 = &DAT_00d386e0;
          for (uVar6 = uVar7 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
            *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
            pcVar8 = pcVar8 + 4;
            pcVar9 = pcVar9 + 4;
          }
          for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
            *pcVar9 = *pcVar8;
            pcVar8 = pcVar8 + 1;
            pcVar9 = pcVar9 + 1;
          }
          FUN_004c5970(iVar5,uVar4,iVar3,iVar1);
        }
      }
    }
    if ((*(int *)(&DAT_007009dc + DAT_00e38ea8 * 0x4c) <= iVar3) &&
       (*(int *)(&DAT_007009dc + DAT_00e38ea8 * 0x4c) < iVar3)) {
      FUN_0041c600((int)DAT_00e38ea8,uVar4,iVar3,iVar1);
    }
  }
  return;
}
