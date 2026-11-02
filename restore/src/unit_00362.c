#include "restore_types.h"
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define PTR_DAT_005b0060 (*(unsigned char * *)0x005b0060u)
extern void FUN_004e6bd0();

void FUN_004210c0(int param_1)

{
  bool bVar1;
  undefined **ppuVar2;
  
  bVar1 = false;
  ppuVar2 = &PTR_DAT_005b0060;
  do {
    if (0x5b014f < (int)ppuVar2) {
      if (!bVar1) {
        FUN_004e6bd0(param_1);
      }
      return;
    }
    if ((ppuVar2[-2] == (undefined *)(&DAT_00799a30)[param_1 * 0x42]) &&
       (0 < *(int *)(*ppuVar2 + 0xc))) {
      bVar1 = true;
    }
    ppuVar2 = ppuVar2 + 4;
  } while (!bVar1);
  return;
}
