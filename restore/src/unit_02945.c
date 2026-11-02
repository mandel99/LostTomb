#include "restore_types.h"
#define DAT_0060496c (*(undefined *)0x0060496cu)
#define DAT_01266408 (*(undefined *)0x01266408u)
#define DAT_012666a0 (*(undefined *)0x012666a0u)
extern void FUN_00567e05();

void FUN_00567dcc(void)

{
  if ((DAT_01266408 == 1) || ((DAT_01266408 == 0 && (DAT_0060496c == 1)))) {
    FUN_00567e05(0xfc);
    if (DAT_012666a0 != (code *)0x0) {
      ((code *)DAT_012666a0)();
    }
    FUN_00567e05(0xff);
  }
  return;
}
