#include "restore_types.h"
#define DAT_00705ea8 (*(undefined4 *)0x00705ea8u)
#define DAT_00705eac (*(undefined4 *)0x00705eacu)
#define DAT_00705eb0 (*(undefined4 *)0x00705eb0u)
#define DAT_00705eb4 (*(undefined4 *)0x00705eb4u)
#define DAT_007064c0 (*(undefined *)0x007064c0u)
#define DAT_007064c4 (*(undefined *)0x007064c4u)
#define DAT_007064c8 (*(undefined *)0x007064c8u)
#define DAT_007064cc (*(undefined *)0x007064ccu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
extern void FUN_00481830();

void FUN_0046a140(void)

{
  int iVar1;
  int iVar2;
  uint *puVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  
  iVar5 = 0;
  iVar2 = DAT_00bca2d8;
  if (0 < DAT_00bca2d4) {
    do {
      iVar4 = 0;
      if (0 < DAT_00bca2d0) {
        puVar3 = &DAT_00af598c + iVar2;
        do {
          if (((&DAT_00b285cc)[iVar2] & 0x80) == 0) {
            uVar6 = DAT_007064c0;
            uVar7 = DAT_007064c4;
            uVar8 = DAT_007064c8;
            uVar9 = DAT_007064cc;
            if ((*puVar3 & 0x20) == 0) {
              if ((*puVar3 & 0x1000) == 0) goto LAB_0046a1fc;
              uVar6 = 0xfffffffe;
              uVar7 = 1;
              uVar8 = 1;
              uVar9 = 2;
            }
LAB_0046a1f0: ;
            FUN_00481830(iVar4,iVar5,1,uVar6,uVar7,uVar8,uVar9);
          }
          else {
            if ((*puVar3 & 0x40) != 0) {
              iVar1 = 0x26;
LAB_0046a192: ;
              iVar1 = iVar1 * 0x28;
              uVar6 = *(undefined4 *)(&DAT_00705ea8 + iVar1);
              uVar7 = *(undefined4 *)(&DAT_00705eac + iVar1);
              uVar8 = *(undefined4 *)(&DAT_00705eb0 + iVar1);
              uVar9 = *(undefined4 *)(&DAT_00705eb4 + iVar1);
              goto LAB_0046a1f0;
            }
            if ((*puVar3 & 2) != 0) {
              iVar1 = 10;
              goto LAB_0046a192;
            }
            (&DAT_00b285cc)[iVar2] = (&DAT_00b285cc)[iVar2] & 0x7f;
          }
LAB_0046a1fc: ;
          iVar4 = iVar4 + 1;
          iVar2 = iVar2 + 1;
          puVar3 = puVar3 + 1;
        } while (iVar4 < DAT_00bca2d0);
      }
      iVar5 = iVar5 + 1;
      iVar2 = iVar2 + DAT_00bca2dc;
    } while (iVar5 < DAT_00bca2d4);
  }
  return;
}
