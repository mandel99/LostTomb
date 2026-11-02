#include "restore_types.h"
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00ea4464 (*(undefined4 *)0x00ea4464u)
#define DAT_00ea4478 (*(undefined4 *)0x00ea4478u)
#define DAT_00ea4484 (*(undefined4 *)0x00ea4484u)
#define DAT_00ea4488 (*(undefined4 *)0x00ea4488u)
#define DAT_00ea5f10 (*(undefined4 *)0x00ea5f10u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a8c0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = DAT_00eaac28;
  iVar3 = DAT_0078d7c0;
  iVar1 = DAT_00eaac28 * 0x1276;
  iVar2 = DAT_00eaac28 * 0x1276;
  (&DAT_00ea4488)[DAT_00eaac28 * 0x1276] = (&DAT_00ea4484)[DAT_00eaac28 * 0x1276];
  (&DAT_00ea5f10)[iVar4 * 0x1276] =
       (0xc - iVar3) * ((int)((&DAT_00ea4478)[iVar1] * (&DAT_00ea4464)[iVar2]) / 0x78) +
       (&DAT_00ea4484)[iVar4 * 0x1276];
  return;
}
