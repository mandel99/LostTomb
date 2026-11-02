#include "restore_types.h"
#define DAT_006fdb38 (*(undefined4 *)0x006fdb38u)
#define DAT_006fdc38 (*(undefined *)0x006fdc38u)
#define DAT_006fdc3c (*(undefined *)0x006fdc3cu)
#define DAT_006fdc40 (*(undefined *)0x006fdc40u)
extern void FUN_00560560();
extern void FUN_00560580();
extern void FUN_005605c0();

void FUN_00416bc0(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  if (DAT_006fdc38 != 0) {
    FUN_00560560(DAT_006fdc38);
    DAT_006fdc40 = 0;
    if (DAT_006fdc3c != 0) {
      FUN_005605c0(DAT_006fdc3c);
    }
    if (DAT_006fdc38 != 0) {
      FUN_00560580(DAT_006fdc38);
    }
    puVar2 = &DAT_006fdb38;
    for (iVar1 = 0x44; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar2 = 0;
      puVar2 = puVar2 + 1;
    }
  }
  return;
}
