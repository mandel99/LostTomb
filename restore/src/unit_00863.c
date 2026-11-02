#include "restore_types.h"
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea4508 (*(undefined4 *)0x00ea4508u)
#define DAT_00ea4510 (*(undefined4 *)0x00ea4510u)
#define DAT_00ea5cc0 (*(undefined4 *)0x00ea5cc0u)
#define DAT_00ea620c (*(undefined4 *)0x00ea620cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a3f0(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_00eaac28;
  iVar2 = DAT_00eaac28 * 0x49d8;
  if ((-5000 < *(int *)(&DAT_00ea187c + iVar2)) && (*(int *)(&DAT_00ea620c + iVar2) != 0)) {
    (&DAT_00ea4510)[DAT_00eaac28 * 0x1276] =
         (&DAT_00ea4510)[DAT_00eaac28 * 0x1276] + (&DAT_00ea4508)[DAT_00eaac28 * 0x1276];
    (&DAT_00ea5cc0)[iVar1 * 0x1276] =
         (&DAT_00ea5cc0)[iVar1 * 0x1276] + (&DAT_00ea4508)[iVar1 * 0x1276];
    *(int *)(&DAT_00ea187c + iVar2) =
         *(int *)(&DAT_00ea187c + iVar2) - (&DAT_00ea4508)[iVar1 * 0x1276];
  }
  return;
}
