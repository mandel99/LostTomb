#include "restore_types.h"
#define _DAT_012663c4 (*(undefined4 *)0x012663c4u)
extern void FUN_005618ae();
extern void FUN_005640ec();
extern void FUN_0056413c();

void FUN_00561896(void)

{
  FUN_005618ae();
  _DAT_012663c4 = (FUN_0056413c(), restore_unresolved_return(0x0056413cu));
  FUN_005640ec();
  return;
}
