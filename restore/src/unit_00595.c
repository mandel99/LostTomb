#include "restore_types.h"
#define DAT_00799aa8 (*(undefined4 *)0x00799aa8u)
#define DAT_00799ab0 (*(undefined4 *)0x00799ab0u)
#define DAT_009ab9ec (*(undefined4 *)0x009ab9ecu)
#define DAT_009b84fc (*(undefined4 *)0x009b84fcu)
#define DAT_009d1b1c (*(undefined4 *)0x009d1b1cu)
#define DAT_009de62c (*(undefined4 *)0x009de62cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ea6010 (*(undefined4 *)0x00ea6010u)
#define DAT_00ea6014 (*(undefined4 *)0x00ea6014u)
#define DAT_00ea6082 (*(undefined4 *)0x00ea6082u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0040f860();
extern undefined4 FUN_0043f410();
extern void FUN_00440060();
extern void FUN_004413b0();
extern int FUN_0046a400();
extern undefined4 FUN_00485100();
extern void FUN_00485830();

undefined4 FUN_00440ed0(int param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  int iVar1;
  int iVar2;
  
  if ((*(int *)(&DAT_00ea6010 + DAT_00eaac28 * 0x49d8) != -1) &&
     (*(int *)(&DAT_00ea6014 + DAT_00eaac28 * 0x49d8) != -1)) {
    return 0;
  }
  iVar1 = FUN_0043f410(0xd1);
  if (iVar1 == 0) {
    return 0;
  }
  if (param_1 != 0) {
    return 1;
  }
  FUN_0040f860(&DAT_009de62c,&DAT_00af598c,0x32c40);
  FUN_0040f860(&DAT_009d1b1c,&DAT_00ae8e7c,0xcb10);
  FUN_0040f860(&DAT_009b84fc,&DAT_00b285cc,0xcb10);
  FUN_0040f860(&DAT_009ab9ec,&DAT_00b67d1c,0xcb10);
  FUN_00485830(0);
  if (DAT_00bca2d0 <= param_4) {
    param_4 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= param_5) {
    param_5 = DAT_00bca2d4 + -1;
  }
  DAT_00bc971c = 1;
  iVar2 = DAT_00eaac28 * 0x49d8;
  iVar1 = DAT_00bca2d8 + param_5 * 0xe4;
  *(int *)(&DAT_00ea6010 + iVar2) = param_4;
  iVar1 = iVar1 + param_4;
  *(int *)(&DAT_00ea6014 + iVar2) = param_5;
  FUN_004413b0(param_4,param_5,0xd1);
  iVar2 = FUN_0046a400(DAT_00eaac28,0xd1,param_4,param_5,0,0);
  FUN_00485100(iVar2);
  (&DAT_00799aa8)[iVar2 * 0x42] = iVar1;
  (&DAT_00799ab0)[iVar2 * 0x42] = 1;
  FUN_00440060(iVar1,0xd1,iVar2);
  *(undefined2 *)((int)&DAT_00ea6082 + DAT_00eaac28 * 0x49d8 + 2) = 1;
  return 1;
}
