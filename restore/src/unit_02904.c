#include "restore_types.h"
#define DAT_01266a74 (*(undefined *)0x01266a74u)
#define DAT_01266a78 (*(undefined *)0x01266a78u)

uint FUN_00565465(int param_1)

{
  uint uVar1;
  
  uVar1 = DAT_01266a78;
  while( true ) {
    if (DAT_01266a78 + DAT_01266a74 * 0x14 <= uVar1) {
      return 0;
    }
    if ((uint)(param_1 - *(int *)(uVar1 + 0xc)) < 0x100000) break;
    uVar1 = uVar1 + 0x14;
  }
  return uVar1;
}
