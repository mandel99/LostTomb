#include "restore_types.h"
#define DAT_00ea4484 (*(undefined4 *)0x00ea4484u)
#define DAT_00ea448c (*(undefined4 *)0x00ea448cu)
#define DAT_00ea5d4c (*(undefined4 *)0x00ea5d4cu)
#define DAT_00ea5e18 (*(undefined4 *)0x00ea5e18u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a450(void)

{
  int iVar1;
  
  iVar1 = DAT_00eaac28;
  (&DAT_00ea448c)[DAT_00eaac28 * 0x1276] = (&DAT_00ea4484)[DAT_00eaac28 * 0x1276];
  (&DAT_00ea4484)[iVar1 * 0x1276] = 0;
  (&DAT_00ea5e18)[iVar1 * 0x1276] = (&DAT_00ea5d4c)[iVar1 * 0x1276];
  (&DAT_00ea5d4c)[iVar1 * 0x1276] = 0;
  return;
}
