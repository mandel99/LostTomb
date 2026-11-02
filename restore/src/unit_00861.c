#include "restore_types.h"
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea4464 (*(undefined4 *)0x00ea4464u)
#define DAT_00ea4478 (*(undefined4 *)0x00ea4478u)
#define DAT_00ea4484 (*(undefined4 *)0x00ea4484u)
#define DAT_00ea5d4c (*(undefined4 *)0x00ea5d4cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a2e0(void)

{
  int iVar1;
  int iVar2;
  
  iVar2 = DAT_00eaac28;
  iVar1 = (int)((&DAT_00ea4464)[DAT_00eaac28 * 0x1276] * (&DAT_00ea4478)[DAT_00eaac28 * 0x1276]) /
          0x78;
  *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) =
       *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) - iVar1;
  (&DAT_00ea4484)[iVar2 * 0x1276] = (&DAT_00ea4484)[iVar2 * 0x1276] + iVar1;
  (&DAT_00ea5d4c)[iVar2 * 0x1276] =
       (&DAT_00ea5d4c)[iVar2 * 0x1276] + (&DAT_00ea4478)[iVar2 * 0x1276];
  return;
}
