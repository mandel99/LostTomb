#include "restore_types.h"
#define DAT_005d2e88 (*(undefined *)0x005d2e88u)
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785d74 (*(undefined4 *)0x00785d74u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00f535d0 (*(undefined4 *)0x00f535d0u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
extern void FUN_004d6160();

void FUN_0052e610(void)

{
  int iVar1;
  int iVar2;
  undefined2 *puVar3;
  
  if (DAT_00e38dda == '\x14') {
    iVar1 = (&DAT_00f535d0)[DAT_005d2e88 * 5];
    iVar2 = (DAT_00e38db0 + -0xf0) / 2;
    if ((iVar1 < 0) || (DAT_00785d70 <= iVar1)) {
      puVar3 = (undefined2 *)0x0;
    }
    else {
      puVar3 = &DAT_00785d74 + iVar1 * 0x3e;
    }
    if (*(char *)(puVar3 + 1) == '\x01') {
      if (puVar3[0x18] == 5) {
        FUN_004d6160(0x2f,0x10,iVar2,DAT_00e38dac + -0x44,0xf0,DAT_00f56f8c,0,DAT_00e38da8,
                     DAT_00e38da4);
        return;
      }
      FUN_004d6160(0x2f,0x11,iVar2,DAT_00e38dac + -0x44,0xf0,DAT_00f56f8c,0,DAT_00e38da8,
                   DAT_00e38da4);
      return;
    }
    FUN_004d6160(0x2f,0xf,iVar2,DAT_00e38dac + -0x44,0xf0,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  }
  return;
}
