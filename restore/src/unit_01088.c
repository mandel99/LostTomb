#include "restore_types.h"
#define DAT_005ca658 (*(undefined4 *)0x005ca658u)
#define DAT_005ca958 (*(undefined4 *)0x005ca958u)
#define DAT_005cac58 (*(undefined4 *)0x005cac58u)
#define DAT_005cbea8 (*(undefined4 *)0x005cbea8u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define PTR_DAT_005cbdc8 (*(unsigned char * *)0x005cbdc8u)
extern void FUN_00475bf0();
extern void FUN_00475cc0();
extern void FUN_00475dc0();
extern void FUN_004767d0();
extern void FUN_004770f0();
extern void FUN_00477500();
extern void FUN_004776f0();
extern void FUN_004777c0();
extern void FUN_00484ee0();
extern void FUN_0051f780();

void FUN_00479510(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  FUN_00484ee0(&DAT_005ca658,0x30);
  FUN_00484ee0(&DAT_005ca958,0x30);
  FUN_00484ee0(&DAT_005cac58,0x18);
  FUN_00484ee0(&PTR_DAT_005cbdc8,0xe);
  FUN_00484ee0(&DAT_005cbea8,0x11);
  FUN_0051f780(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_00475bf0(param_1,param_2,param_3,param_4);
  FUN_00477500(param_1,param_2,param_3,param_4);
  FUN_0051f780(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_004767d0(param_1,param_2,param_3,param_4,0,0);
  return;
}
