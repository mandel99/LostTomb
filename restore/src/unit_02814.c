#include "restore_types.h"
#define DAT_00604e64 (*(undefined *)0x00604e64u)
#define PTR_DAT_00604c58 (*(unsigned char * *)0x00604c58u)
extern uint FUN_00565f66();

uint FUN_00561d89(int param_1)

{
  uint uVar1;
  
  if (1 < DAT_00604e64) {
    uVar1 = FUN_00565f66(param_1,4);
    return uVar1;
  }
  return (byte)((undefined4 *)&PTR_DAT_00604c58)[param_1 * 2] & 4;
}
