#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define PTR_DAT_005a8148 (*(unsigned char * *)0x005a8148u)
extern void FUN_00401be0();
extern void FUN_00401d30();

void FUN_00420a90(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  bool bVar2;
  undefined **ppuVar3;
  
  bVar2 = false;
  puVar1 = *(undefined **)
            (&DAT_00799a2c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84);
  FUN_00401d30(param_1,param_2);
  ppuVar3 = &PTR_DAT_005a8148;
  do {
    if (0x5a84c7 < (int)ppuVar3) {
      if (!bVar2) {
        FUN_00401be0(param_1);
      }
      return;
    }
    if ((ppuVar3[-2] == puVar1) && (0 < *(int *)(*ppuVar3 + 0xc))) {
      bVar2 = true;
    }
    ppuVar3 = ppuVar3 + 4;
  } while (!bVar2);
  return;
}
