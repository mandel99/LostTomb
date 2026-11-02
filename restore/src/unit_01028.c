#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_009d1b1c (*(undefined4 *)0x009d1b1cu)
#define DAT_009de62c (*(undefined4 *)0x009de62cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
extern void FUN_0040f860();
extern void FUN_004746d0();
extern void FUN_00485830();
extern void FUN_00516f90();
extern void FUN_005173a0();
extern undefined4 FUN_0051b680();
extern undefined4 FUN_0051bc60();
extern undefined4 FUN_0051c930();

void FUN_00474f50(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  
  FUN_0040f860(&DAT_009de62c,&DAT_00af598c,0x32c40);
  FUN_0040f860(&DAT_009d1b1c,&DAT_00ae8e7c,0xcb10);
  FUN_00485830(0);
  DAT_00bc971c = 0;
  iVar1 = DAT_00bca2d8 + param_3 * 0xe4 + param_2;
  if ((((((&DAT_00af598c)[iVar1] & 8) == 0) ||
       ((*(short *)(&DAT_00b4e6fc + iVar1 * 2) != 0 &&
        ((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + iVar1 * 2) * 0x84] == 0x8a)))) &&
      (((&DAT_00af598c)[iVar1] & 0x26345237) == 0)) &&
     ((((&DAT_00af598c)[DAT_00bca2d8 + param_5 * 0xe4 + param_4] & 8) == 0 &&
      (((&DAT_00af598c)[DAT_00bca2d8 + param_5 * 0xe4 + param_4] & 0x26345237) == 0)))) {
    if (DAT_00605a3c == 0) {
      iVar1 = FUN_0051bc60(0,param_2,param_3,0);
    }
    else {
      iVar1 = FUN_0051b680();
    }
    if (((iVar1 != 0) && (iVar1 = FUN_0051c930(0,param_2,param_3,param_4,param_5,0), iVar1 != 0)) &&
       (param_1 == 0)) {
      FUN_00516f90(param_2,param_3,param_4,param_5,1);
      FUN_005173a0(param_2,param_3,param_4,param_5);
      FUN_004746d0(param_2,param_3,param_4 - param_2);
      DAT_00e38dca = 1;
    }
  }
  return;
}
