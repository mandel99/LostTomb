#include "restore_types.h"
#define DAT_00bc9800 (*(undefined *)0x00bc9800u)
#define DAT_00bca310 (*(undefined *)0x00bca310u)
extern undefined4 FUN_00485100();
extern void FUN_004e2970();

void FUN_00486020(void)

{
  bool bVar1;
  
  if (DAT_00bca310 == 0) {
    DAT_00bca310 = 1;
    FUN_004e2970(0x43,0,0);
    return;
  }
  bVar1 = DAT_00bca310 == 3;
  DAT_00bca310 = 0;
  if (bVar1) {
    FUN_00485100(DAT_00bc9800);
  }
  return;
}
