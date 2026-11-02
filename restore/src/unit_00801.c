#include "restore_types.h"
#define DAT_0070cdcc (*(undefined4 *)0x0070cdccu)
#define DAT_0070cdd0 (*(undefined4 *)0x0070cdd0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a96 (*(undefined4 *)0x00799a96u)

uint FUN_00453af0(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = (int)(short)(&DAT_007999ea)[param_1 * 0x84];
  iVar2 = *(int *)(&DAT_0070cdd0 + iVar1 * 0x58);
  if (0x12 < iVar1) {
    iVar2 = 1000;
  }
  if ((int)(char)(&DAT_00799a96)[param_1 * 0x108] <= (int)(&DAT_0070cdcc)[iVar1 * 0x16]) {
    return 0xffffffff;
  }
  return (uint)(iVar2 <= (char)(&DAT_00799a96)[param_1 * 0x108]);
}
