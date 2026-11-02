#include "restore_types.h"
#define DAT_00705894 (*(undefined *)0x00705894u)
#define DAT_00705898 (*(undefined *)0x00705898u)
#define DAT_00705a28 (*(undefined *)0x00705a28u)
#define DAT_00705a2c (*(undefined *)0x00705a2cu)
#define DAT_00705a30 (*(undefined *)0x00705a30u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
extern int FUN_0046f1c0();

int FUN_004347b0(int param_1)

{
  int iVar1;
  
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    return 0;
  }
  if (DAT_00705a30 + DAT_00705a2c + DAT_00705a28 + DAT_00705898 + DAT_00705894 == 0) {
    return 0;
  }
  iVar1 = FUN_0046f1c0(param_1);
  return iVar1 / 0x19;
}
