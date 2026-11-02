#include "restore_types.h"
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_004c5120();

byte FUN_004c4160(int param_1)

{
  int iVar1;
  
  if ((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) {
    iVar1 = FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]);
    if (iVar1 == 3) {
      return (-(param_1 != 0) & 0x19U) + 10;
    }
  }
  return -(param_1 != 0) & 0x19;
}
