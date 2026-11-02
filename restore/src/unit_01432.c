#include "restore_types.h"
#define DAT_005f80fc (*(undefined4 *)0x005f80fcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a9c (*(undefined4 *)0x00799a9cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd632 (*(undefined4 *)0x00bcd632u)
#define DAT_00bcd634 (*(undefined4 *)0x00bcd634u)
#define DAT_00bcd636 (*(undefined4 *)0x00bcd636u)
#define DAT_00c8ad38 (*(undefined4 *)0x00c8ad38u)

undefined4 FUN_004b3470(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  param_1 = param_1 * 0x184;
  iVar1 = (int)*(short *)(&DAT_00bcd634 + param_1);
  iVar2 = *(int *)(&DAT_00bcd618 + param_1);
  while( true ) {
    if (*(short *)(&DAT_00bcd636 + param_1) <= iVar1) {
      return 2;
    }
    if (((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + iVar2 * 2) * 0x84] == 0x88) &&
       ((int)*(short *)(&DAT_00b4e6fc + iVar2 * 2) == (&DAT_00799a9c)[param_2 * 0x42])) break;
    iVar2 = iVar2 + (&DAT_005f80fc)
                    [*(byte *)((int)&DAT_00c8ad38 +
                              *(short *)(&DAT_00bcd632 + param_1) * 500 + iVar1)];
    iVar1 = iVar1 + 1;
  }
  if ((0 < (short)(&DAT_00799a18)[param_2 * 0x84]) &&
     (0 < (short)(&DAT_00799a18)[(&DAT_00799a9c)[param_2 * 0x42] * 0x84])) {
    return 1;
  }
  return 0;
}
