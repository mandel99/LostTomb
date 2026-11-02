#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00705b78 (*(undefined *)0x00705b78u)
#define DAT_00705b7c (*(undefined *)0x00705b7cu)
#define DAT_00705b80 (*(undefined *)0x00705b80u)
extern void FUN_004e2970();

void FUN_004e5070(int param_1)

{
  if ((DAT_00605a3c != 1) && (((param_1 == 0x1f || (param_1 == 0x21)) || (param_1 == 0x1e)))) {
    if (DAT_00705b80 < 1) {
      FUN_004e2970(0xd0,0,0);
    }
    if ((DAT_00705b78 < 1) && (param_1 != 0x1f)) {
      FUN_004e2970(0xd1,0,0);
    }
    if ((DAT_00705b7c < 1) && (param_1 == 0x21)) {
      FUN_004e2970(0xd2,0,0);
    }
  }
  return;
}
