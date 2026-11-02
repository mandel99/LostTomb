#include "restore_types.h"
#define DAT_01266a84 (*(undefined *)0x01266a84u)
extern undefined4 FUN_00569831();

void FUN_00569bf5(void)

{
  if (DAT_01266a84 == 0) {
    FUN_00569831(0xfffffffd);
    DAT_01266a84 = 1;
  }
  return;
}
