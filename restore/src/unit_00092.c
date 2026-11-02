#include "restore_types.h"
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00ea60a4 (*(undefined4 *)0x00ea60a4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)

void FUN_0040c760(void)

{
  int iVar1;
  int iVar2;
  
  if (DAT_00d3b198 == 0) {
    iVar1 = (&DAT_00ea60a4)[DAT_00eaac28 * 0x1276];
    iVar2 = DAT_00eaac28 * 0x1276;
    (&DAT_00ea60a4)[iVar2] = iVar1 + -1;
    if (iVar1 + -1 < 1) {
      (&DAT_00ea60a4)[iVar2] = 1;
      DAT_00e38dca = 1;
      return;
    }
  }
  else {
    iVar2 = DAT_00eaac28 * 0x1276;
    iVar1 = (&DAT_00ea60a4)[DAT_00eaac28 * 0x1276];
    (&DAT_00ea60a4)[iVar2] = iVar1 + 1;
    if (DAT_00f53e6c < iVar1 + 0xe) {
      (&DAT_00ea60a4)[iVar2] = DAT_00f53e6c + -0xd;
    }
  }
  DAT_00e38dca = 1;
  return;
}
