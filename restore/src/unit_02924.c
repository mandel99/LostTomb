#include "restore_types.h"
#define DAT_01266568 (*(undefined *)0x01266568u)
extern void FUN_00566bc3();

void FUN_00566bae(void)

{
  if (DAT_01266568 == 0) {
    FUN_00566bc3();
    DAT_01266568 = DAT_01266568 + 1;
  }
  return;
}
