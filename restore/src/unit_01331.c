#include "restore_types.h"
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
extern void FUN_00495d30();
extern uint FUN_00495d70();

void FUN_00495de0(int param_1)

{
  char cVar1;
  
  cVar1 = FUN_00495d70(param_1);
  if (cVar1 != '\0') {
    FUN_00495d30(param_1);
    return;
  }
  (&DAT_00bcd606)[param_1 * 0x184] = 2;
  return;
}
