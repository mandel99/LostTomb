#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define PTR_DAT_005a8148 (*(unsigned char * *)0x005a8148u)

int FUN_00420c40(int param_1,int param_2)

{
  undefined **ppuVar1;
  
  ppuVar1 = &PTR_DAT_005a8148;
  do {
    if ((*(short *)(*ppuVar1 + 0x12) == 0) &&
       (ppuVar1[-2] ==
        *(undefined **)(&DAT_00799a2c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84)))
    {
      if ((ppuVar1[-1] == (undefined *)(uint)(byte)(&DAT_00bcd602)[param_2 * 0x184]) &&
         (0 < *(int *)(*ppuVar1 + 0xc))) {
        return param_1;
      }
    }
    ppuVar1 = ppuVar1 + 4;
    if (0x5a84c7 < (int)ppuVar1) {
      return 0;
    }
  } while( true );
}
