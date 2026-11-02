#include "restore_types.h"
#define DAT_005d1974 (*(undefined4 *)0x005d1974u)
#define DAT_0070ce18 (*(undefined4 *)0x0070ce18u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00799af2 (*(undefined4 *)0x00799af2u)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00ea1878 (*(undefined4 *)0x00ea1878u)
#define DAT_00ea44ac (*(undefined4 *)0x00ea44acu)
#define DAT_00ea44b0 (*(undefined4 *)0x00ea44b0u)
#define DAT_00ea44bc (*(undefined4 *)0x00ea44bcu)
#define DAT_00ea44c4 (*(undefined4 *)0x00ea44c4u)
#define DAT_00ea44cc (*(undefined4 *)0x00ea44ccu)
#define DAT_00ea5e5c (*(undefined4 *)0x00ea5e5cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004cde40();

void FUN_00459d20(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar1 = DAT_00eaac28;
  iVar8 = DAT_0078d790;
  iVar6 = DAT_00eaac28 * 0x49d8;
  (&DAT_00ea44c4)[DAT_00eaac28 * 0x1276] = 0;
  (&DAT_00ea44cc)[iVar1 * 0x1276] = 0;
  if (0 < iVar8) {
    psVar5 = &DAT_00799af2;
    do {
      if (((char)psVar5[-9] == '\x01') && (*(char *)((int)psVar5 + -0xd) != '\0')) {
        if ((char)psVar5[0x1e] != '\0') {
          if (*psVar5 < 0xe) {
            (&DAT_00ea44c4)[iVar1 * 0x1276] =
                 (&DAT_00ea44c4)[iVar1 * 0x1276] +
                 (int)psVar5[5] * *(int *)(&DAT_0070ce18 + *psVar5 * 0x58);
          }
          else {
            (&DAT_00ea44cc)[iVar1 * 0x1276] =
                 (&DAT_00ea44cc)[iVar1 * 0x1276] +
                 (int)psVar5[5] * *(int *)(&DAT_0070ce18 + *psVar5 * 0x58);
          }
        }
      }
      psVar5 = psVar5 + 0x84;
      iVar8 = iVar8 + -1;
    } while (iVar8 != 0);
  }
  uVar2 = FUN_004cde40((&DAT_00ea44cc)[iVar1 * 0x1276],*(undefined4 *)(&DAT_00ea1878 + iVar6));
  iVar3 = FUN_004cde40(uVar2,*(undefined4 *)(&DAT_005d1974 + DAT_00e39080 * 4));
  uVar2 = FUN_004cde40((&DAT_00ea44c4)[DAT_00eaac28 * 0x1276],
                       *(undefined4 *)(&DAT_00ea1878 + DAT_00eaac28 * 0x49d8));
  iVar4 = FUN_004cde40(uVar2,*(undefined4 *)(&DAT_005d1974 + DAT_00e39080 * 4));
  iVar6 = DAT_00eaac28;
  iVar8 = DAT_00eaac28 * 0x1276;
  iVar1 = DAT_00eaac28 * 0x1276;
  iVar7 = 0xc - DAT_0078d7c0;
  (&DAT_00ea44bc)[DAT_00eaac28 * 0x1276] = (&DAT_00ea44ac)[iVar8] + (&DAT_00ea44b0)[iVar1];
  (&DAT_00ea5e5c)[iVar6 * 0x1276] =
       (iVar4 + iVar3) * iVar7 + (&DAT_00ea44ac)[iVar8] + (&DAT_00ea44b0)[iVar1];
  return;
}
