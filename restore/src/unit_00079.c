#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0078d828 (*(undefined *)0x0078d828u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
extern void FUN_00409fa0();
extern void FUN_0040a090();
extern void FUN_004e2970();

void FUN_0040b590(void)

{
  if (((DAT_00605a3c != 1) && (DAT_00e38ebc == 0)) && (DAT_0078d828 != 0)) {
    FUN_004e2970(0xd,0,0);
    return;
  }
  if (DAT_00e38dda == '\x01') {
    FUN_00409fa0();
    return;
  }
  FUN_0040a090();
  return;
}
