#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define PTR_DAT_0059edf0 (*(unsigned char * *)0x0059edf0u)
extern void FUN_0040ecf0();

void FUN_00420770(int param_1)

{
  bool bVar1;
  undefined **ppuVar2;
  
  bVar1 = false;
  ppuVar2 = &PTR_DAT_0059edf0;
  do {
    if (0x59ef9f < (int)ppuVar2) {
      if (!bVar1) {
        FUN_0040ecf0(param_1);
      }
      return;
    }
    if ((ppuVar2[-2] ==
         (undefined *)(&DAT_00799a30)[(param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x42]) &&
       (0 < *(int *)(*ppuVar2 + 0xc))) {
      bVar1 = true;
    }
    ppuVar2 = ppuVar2 + 4;
  } while (!bVar1);
  return;
}
