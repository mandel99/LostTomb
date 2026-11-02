#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65c (*(undefined4 *)0x00bcd65cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0046cf40();
extern int FUN_004b4800();
extern void FUN_004e90f0();

void FUN_00458d80(int param_1,int param_2)

{
  int iVar1;
  
  FUN_004e90f0(DAT_00eaac28,-param_2);
  if (param_2 < (short)(&DAT_007999f4)[param_1 * 0x84]) {
    (&DAT_007999f4)[param_1 * 0x84] = (&DAT_007999f4)[param_1 * 0x84] - (short)param_2;
  }
  else {
    (&DAT_007999f4)[param_1 * 0x84] = 0;
    FUN_0046cf40(param_1);
  }
  iVar1 = FUN_004b4800(DAT_00eaac28,2,(int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2),
                       (int)*(short *)(&DAT_007999e0 + param_1 * 0x108),0);
  iVar1 = iVar1 * 0x184;
  (&DAT_00bcd630)[iVar1] = 8;
  *(undefined2 *)(&DAT_00bcd62e + iVar1) = 0;
  (&DAT_00bcd65c)[iVar1] = (char)param_2;
  return;
}
