#include "restore_types.h"
#define DAT_005d2540 (*(undefined *)0x005d2540u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)

undefined4 FUN_00434a60(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_005d2540;
  if (DAT_005d2540 != 0) {
    iVar1 = DAT_005d2540;
    do {
      if ((short)(&DAT_007999e8)[param_1 * 0x84] == iVar1) {
        return 1;
      }
      iVar1 = puVar2[1];
      puVar2 = puVar2 + 1;
    } while (iVar1 != 0);
  }
  return 0;
}
