#include "restore_types.h"
#define DAT_005700d4 (*(undefined4 *)0x005700d4u)
#define DAT_005700e8 (*(undefined4 *)0x005700e8u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
extern undefined4 FUN_00485100();

void FUN_004f5e70(int param_1,char param_2)

{
  int iVar1;
  
  if (DAT_005e4ef4 == 5) {
    iVar1 = *(int *)(&DAT_005700e8 +
                    (uint)*(byte *)(&DAT_00799a2c +
                                   (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84) * 4);
  }
  else {
    iVar1 = *(int *)(&DAT_005700d4 +
                    (uint)*(byte *)(&DAT_00799a2c +
                                   (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84) * 4);
  }
  iVar1 = (int)*(short *)(&DAT_00b4e6fc +
                         (*(int *)(&DAT_007999e4 +
                                  (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x108) +
                         iVar1) * 2);
  if ((iVar1 != 0) && ((&DAT_007999e8)[iVar1 * 0x84] == 0xd7)) {
    if (param_2 != '\0') {
      FUN_00485100(iVar1);
      (&DAT_007999d8)[iVar1 * 0x108] = 6;
      DAT_00bc971c = DAT_00bc971c + 1;
      return;
    }
    (&DAT_007999d8)[iVar1 * 0x108] = 2;
  }
  return;
}
