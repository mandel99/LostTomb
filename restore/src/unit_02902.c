#include "restore_types.h"
#define DAT_01266a7c (*(undefined *)0x01266a7cu)
extern undefined4 FUN_00565427();

undefined4 FUN_005653eb(int param_1)

{
  int iVar1;
  
  DAT_01266a7c = HeapCreate((uint)(param_1 == 0),0x1000,0);
  if (DAT_01266a7c != (HANDLE)0x0) {
    iVar1 = FUN_00565427();
    if (iVar1 != 0) {
      return 1;
    }
    HeapDestroy(DAT_01266a7c);
  }
  return 0;
}
