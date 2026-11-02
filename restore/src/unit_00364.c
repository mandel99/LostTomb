#include "restore_types.h"
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define PTR_DAT_005b0060 (*(unsigned char * *)0x005b0060u)

int FUN_00421230(int param_1,int param_2)

{
  undefined **ppuVar1;
  
  ppuVar1 = &PTR_DAT_005b0060;
  do {
    if ((*(short *)(*ppuVar1 + 0x12) == 0) &&
       (ppuVar1[-2] == (undefined *)(&DAT_00799a30)[param_1 * 0x42])) {
      if ((ppuVar1[-1] == (undefined *)(uint)(byte)(&DAT_00bcd602)[param_2 * 0x184]) &&
         (0 < *(int *)(*ppuVar1 + 0xc))) {
        return param_1;
      }
    }
    ppuVar1 = ppuVar1 + 4;
    if (0x5b014f < (int)ppuVar1) {
      return 0;
    }
  } while( true );
}
