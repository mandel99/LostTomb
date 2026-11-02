#include "restore_types.h"
#define DAT_00ea5cc0 (*(undefined4 *)0x00ea5cc0u)
#define DAT_00ea5de4 (*(undefined4 *)0x00ea5de4u)
#define DAT_00ea5de8 (*(undefined4 *)0x00ea5de8u)
#define DAT_00ea5dec (*(undefined4 *)0x00ea5decu)
#define DAT_00eaac28 (*(undefined4 *)0x00eaac28u)

void FUN_004e7db0(void)

{
  int iVar1;
  
  iVar1 = DAT_00eaac28;
  (&DAT_00ea5de4)[DAT_00eaac28 * 0x1276] =
       ((int)((&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276] +
             ((int)(&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276] >> 0x1f & 7U)) >> 3) + 0x14;
  (&DAT_00ea5de8)[iVar1 * 0x1276] =
       ((int)((&DAT_00ea5cc0)[iVar1 * 0x1276] + ((int)(&DAT_00ea5cc0)[iVar1 * 0x1276] >> 0x1f & 3U))
       >> 2) + 0x32;
  (&DAT_00ea5dec)[iVar1 * 0x1276] = (int)(&DAT_00ea5cc0)[iVar1 * 0x1276] / 2 + 100;
  return;
}
