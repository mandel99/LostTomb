#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
extern undefined4 FUN_0047fe60();

undefined4 FUN_004e7c90(int param_1)

{
  short sVar1;
  int iVar2;
  
  sVar1 = (&DAT_007999e8)[param_1 * 0x84];
  if ((((((0x3b < sVar1) && (sVar1 < 0x41)) || (sVar1 == 0x2f)) ||
       ((sVar1 == 0xb7 || (sVar1 == 0xde)))) ||
      ((sVar1 == 0xe5 || ((sVar1 == 0xea || (sVar1 == 0xeb)))))) || (sVar1 == 0xec)) {
    iVar2 = FUN_0047fe60((int)(short)(&DAT_007999fe)[param_1 * 0x84],
                         (int)(short)(&DAT_00799a00)[param_1 * 0x84],1,6);
    if (iVar2 != 0) {
      return 1;
    }
  }
  return 0;
}
