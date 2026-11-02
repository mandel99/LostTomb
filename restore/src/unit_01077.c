#include "restore_types.h"
#define DAT_005ca658 (*(undefined4 *)0x005ca658u)
#define DAT_005ca958 (*(undefined4 *)0x005ca958u)
#define DAT_005cac58 (*(undefined4 *)0x005cac58u)
#define DAT_005cbea8 (*(undefined4 *)0x005cbea8u)
extern void FUN_00475cc0();
extern void FUN_00475dc0();
extern void FUN_004770f0();
extern void FUN_00477430();
extern void FUN_004776f0();
extern void FUN_004777c0();
extern void FUN_0047b590();
extern void FUN_0047bd50();
extern void FUN_00484ee0();

void FUN_00478ea0(int param_1,int param_2,int param_3,int param_4)

{
  FUN_00484ee0(&DAT_005ca658,0x30);
  FUN_00484ee0(&DAT_005ca958,0x30);
  FUN_00484ee0(&DAT_005cac58,0x18);
  FUN_00484ee0(&DAT_005cbea8,0x11);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00477430(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_0047b590(param_1 + -1,param_2 + -1,param_3 + 1,param_4 + 1);
  FUN_0047bd50(param_1 + -1,param_2 + -1,param_3 + 1,param_4 + 1);
  return;
}
