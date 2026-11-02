#include "restore_types.h"
#define DAT_005d1974 (*(undefined4 *)0x005d1974u)
#define DAT_0070ce18 (*(undefined4 *)0x0070ce18u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af2 (*(undefined4 *)0x00799af2u)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00ea1878 (*(undefined4 *)0x00ea1878u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea3efc (*(undefined4 *)0x00ea3efcu)
#define DAT_00ea4490 (*(undefined4 *)0x00ea4490u)
#define DAT_00ea4494 (*(undefined4 *)0x00ea4494u)
#define DAT_00ea4498 (*(undefined4 *)0x00ea4498u)
#define DAT_00ea449c (*(undefined4 *)0x00ea449cu)
#define DAT_00ea44a0 (*(undefined4 *)0x00ea44a0u)
#define DAT_00ea44a4 (*(undefined4 *)0x00ea44a4u)
#define DAT_00ea44a8 (*(undefined4 *)0x00ea44a8u)
#define DAT_00ea44ac (*(undefined4 *)0x00ea44acu)
#define DAT_00ea44b0 (*(undefined4 *)0x00ea44b0u)
#define DAT_00ea44b4 (*(undefined4 *)0x00ea44b4u)
#define DAT_00ea44b8 (*(undefined4 *)0x00ea44b8u)
#define DAT_00ea44c4 (*(undefined4 *)0x00ea44c4u)
#define DAT_00ea44c8 (*(undefined4 *)0x00ea44c8u)
#define DAT_00ea44cc (*(undefined4 *)0x00ea44ccu)
#define DAT_00ea44d0 (*(undefined4 *)0x00ea44d0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004cde40();
extern int FUN_004cde60();

void FUN_004599d0(void)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  short *psVar8;
  undefined4 *puVar9;
  int local_4;
  
  iVar7 = DAT_00eaac28;
  iVar2 = DAT_00eaac28 * 0x49d8;
  (&DAT_00ea4490)[DAT_00eaac28 * 0x1276] = 0;
  (&DAT_00ea4494)[iVar7 * 0x1276] = 0;
  (&DAT_00ea4498)[iVar7 * 0x1276] = 0;
  (&DAT_00ea449c)[iVar7 * 0x1276] = 0;
  (&DAT_00ea44c8)[iVar7 * 0x1276] = 0;
  (&DAT_00ea44c4)[iVar7 * 0x1276] = 0;
  (&DAT_00ea44d0)[iVar7 * 0x1276] = 0;
  (&DAT_00ea44cc)[iVar7 * 0x1276] = 0;
  puVar9 = (undefined4 *)(&DAT_00ea3efc + iVar2);
  for (iVar4 = 0x14; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar9 = 0;
    puVar9 = puVar9 + 1;
  }
  local_4 = 1;
  if (0 < DAT_0078d790) {
    psVar8 = &DAT_00799af2;
    do {
      if (((char)psVar8[-9] == '\x01') && (*(char *)((int)psVar8 + -0xd) != '\0')) {
        bVar1 = *psVar8 < 0xe;
        iVar2 = *(int *)(&DAT_0070ce18 + *psVar8 * 0x58);
        if ((char)psVar8[0x3b] != '\0') {
          iVar2 = FUN_004cde40(iVar2,0x6e);
          iVar7 = DAT_00eaac28;
        }
        iVar4 = (int)psVar8[5];
        *(int *)(&DAT_00ea3efc + (iVar7 * 0x1276 + (int)*psVar8) * 4) =
             *(int *)(&DAT_00ea3efc + (iVar7 * 0x1276 + (int)*psVar8) * 4) + iVar4;
        if ((char)psVar8[0x1e] == '\0') {
          if (bVar1) {
            (&DAT_00ea4498)[iVar7 * 0x1276] = (&DAT_00ea4498)[iVar7 * 0x1276] + iVar4;
            (&DAT_00ea44c8)[iVar7 * 0x1276] = (&DAT_00ea44c8)[iVar7 * 0x1276] + iVar4 * iVar2;
          }
          else {
            (&DAT_00ea449c)[iVar7 * 0x1276] = (&DAT_00ea449c)[iVar7 * 0x1276] + iVar4;
            (&DAT_00ea44d0)[iVar7 * 0x1276] = (&DAT_00ea44d0)[iVar7 * 0x1276] + iVar4 * iVar2;
          }
        }
        else {
          if (bVar1) {
            (&DAT_00ea4490)[iVar7 * 0x1276] = (&DAT_00ea4490)[iVar7 * 0x1276] + iVar4;
          }
          else {
            (&DAT_00ea4494)[iVar7 * 0x1276] = (&DAT_00ea4494)[iVar7 * 0x1276] + iVar4;
          }
          iVar4 = iVar4 * iVar2;
          *(int *)(psVar8 + 0x3d) =
               *(int *)(psVar8 + 0x3d) +
               *(int *)(&DAT_005d1974 + DAT_00e39080 * 4) * *(int *)(&DAT_00ea1878 + iVar7 * 0x49d8)
               * iVar4;
          if (bVar1) {
            (&DAT_00ea44c4)[iVar7 * 0x1276] = (&DAT_00ea44c4)[iVar7 * 0x1276] + iVar4;
          }
          else {
            (&DAT_00ea44cc)[iVar7 * 0x1276] = (&DAT_00ea44cc)[iVar7 * 0x1276] + iVar4;
          }
        }
      }
      local_4 = local_4 + 1;
      psVar8 = psVar8 + 0x84;
    } while (local_4 <= DAT_0078d790);
  }
  uVar3 = FUN_004cde40((&DAT_00ea44cc)[iVar7 * 0x1276],
                       *(undefined4 *)(&DAT_00ea1878 + iVar7 * 0x49d8));
  iVar2 = FUN_004cde40(uVar3,*(undefined4 *)(&DAT_005d1974 + DAT_00e39080 * 4));
  iVar7 = DAT_00eaac28;
  iVar4 = DAT_00eaac28 * 0x49d8;
  (&DAT_00ea44b0)[DAT_00eaac28 * 0x1276] = (&DAT_00ea44b0)[DAT_00eaac28 * 0x1276] + iVar2;
  uVar3 = FUN_004cde40((&DAT_00ea44c4)[iVar7 * 0x1276],*(undefined4 *)(&DAT_00ea1878 + iVar4));
  iVar4 = FUN_004cde40(uVar3,*(undefined4 *)(&DAT_005d1974 + DAT_00e39080 * 4));
  iVar7 = DAT_00eaac28;
  iVar5 = DAT_00eaac28 * 0x49d8;
  (&DAT_00ea44ac)[DAT_00eaac28 * 0x1276] = (&DAT_00ea44ac)[DAT_00eaac28 * 0x1276] + iVar4;
  uVar3 = FUN_004cde40((&DAT_00ea44d0)[iVar7 * 0x1276],*(undefined4 *)(&DAT_00ea1878 + iVar5));
  iVar5 = FUN_004cde40(uVar3,*(undefined4 *)(&DAT_005d1974 + DAT_00e39080 * 4));
  iVar7 = DAT_00eaac28;
  iVar6 = DAT_00eaac28 * 0x49d8;
  (&DAT_00ea44b8)[DAT_00eaac28 * 0x1276] = (&DAT_00ea44b8)[DAT_00eaac28 * 0x1276] + iVar5;
  uVar3 = FUN_004cde40((&DAT_00ea44c8)[iVar7 * 0x1276],*(undefined4 *)(&DAT_00ea1878 + iVar6));
  iVar5 = FUN_004cde40(uVar3,*(undefined4 *)(&DAT_005d1974 + DAT_00e39080 * 4));
  iVar7 = DAT_00eaac28;
  iVar6 = DAT_00eaac28 * 0x49d8;
  (&DAT_00ea44b4)[DAT_00eaac28 * 0x1276] = (&DAT_00ea44b4)[DAT_00eaac28 * 0x1276] + iVar5;
  *(int *)(&DAT_00ea187c + iVar6) = *(int *)(&DAT_00ea187c + iVar6) + iVar2 + iVar4;
  uVar3 = FUN_004cde60((&DAT_00ea4490)[iVar7 * 0x1276],
                       (&DAT_00ea4498)[iVar7 * 0x1276] + (&DAT_00ea4490)[iVar7 * 0x1276]);
  (&DAT_00ea44a0)[iVar7 * 0x1276] = uVar3;
  iVar2 = DAT_00eaac28;
  uVar3 = FUN_004cde60((&DAT_00ea4494)[iVar7 * 0x1276],
                       (&DAT_00ea449c)[DAT_00eaac28 * 0x1276] + (&DAT_00ea4494)[iVar7 * 0x1276]);
  (&DAT_00ea44a4)[iVar2 * 0x1276] = uVar3;
  uVar3 = FUN_004cde60((&DAT_00ea4490)[iVar2 * 0x1276] + (&DAT_00ea4494)[iVar2 * 0x1276],
                       (&DAT_00ea449c)[iVar2 * 0x1276] + (&DAT_00ea4498)[iVar2 * 0x1276] +
                       (&DAT_00ea4490)[iVar2 * 0x1276] + (&DAT_00ea4494)[iVar2 * 0x1276]);
  (&DAT_00ea44a8)[DAT_00eaac28 * 0x1276] = uVar3;
  return;
}
