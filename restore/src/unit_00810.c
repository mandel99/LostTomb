#include "restore_types.h"
#define DAT_0078d808 (*(undefined *)0x0078d808u)
extern uint FUN_00562348();

void FUN_00455530(void)

{
  uint uVar1;
  
  uVar1 = FUN_00562348();
  DAT_0078d808 = uVar1 & 7;
  return;
}
