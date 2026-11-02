#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00ea61f0 (*(undefined4 *)0x00ea61f0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0042fb30();
extern void FUN_004b1df0();
extern int FUN_004b4800();

undefined4 FUN_0042fe90(int param_1)

{
  int iVar1;
  
  if (param_1 == 0) {
    return 0;
  }
  FUN_0042fb30(param_1);
  iVar1 = DAT_00eaac28;
  *(int *)(&DAT_00ea61f0 + DAT_00eaac28 * 0x49d8) =
       *(int *)(&DAT_00ea61f0 + DAT_00eaac28 * 0x49d8) + 1;
  iVar1 = FUN_004b4800(iVar1,0x62,(int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2),
                       (int)*(short *)(&DAT_007999e0 + param_1 * 0x108),0);
  if (iVar1 != 0) {
    *(undefined2 *)(&DAT_00bcd62e + iVar1 * 0x184) = 500;
    (&DAT_00bcd652)[iVar1 * 0xc2] = (short)param_1;
    (&DAT_00bcd630)[iVar1 * 0x184] = 8;
    FUN_004b1df0(iVar1);
  }
  return 1;
}
