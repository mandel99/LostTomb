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
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f567ac (*(undefined *)0x00f567acu)
extern void FUN_0040f860();
extern undefined4 FUN_0043f410();
extern void FUN_00440060();
extern uint FUN_00441130();
extern void FUN_004413b0();
extern int FUN_0046a400();
extern void FUN_0047bf50();
extern undefined4 FUN_00485100();
extern void FUN_00485830();

undefined4 FUN_00440700(int param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = param_4;
  if (DAT_00bca2d0 <= param_4) {
    iVar3 = DAT_00bca2d0 + -1;
  }
  iVar4 = param_5;
  if (DAT_00bca2d4 <= param_5) {
    iVar4 = DAT_00bca2d4 + -1;
  }
  iVar5 = DAT_00bca2d8 + iVar4 * 0xe4 + iVar3;
  iVar2 = FUN_0043f410(0x1f);
  if (iVar2 == 0) {
    return 0;
  }
  FUN_004413b0(iVar3,iVar4,0x1f);
  iVar3 = FUN_00441130(iVar3,iVar4,&param_5,&param_4,0x1f,0x1f);
  if (iVar3 == -1) {
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
  DAT_00bc971c = 1;
  iVar3 = FUN_0046a400(DAT_00eaac28,0x1f,param_5,param_4,0,0);
  uVar1 = DAT_00f567ac;
  (&DAT_00799ab0)[iVar3 * 0x42] = 1;
  (&DAT_00799aa8)[iVar3 * 0x42] = iVar5;
  FUN_0047bf50(iVar3,param_5,param_4,1,1,uVar1,8);
  FUN_00485100(iVar3);
  FUN_00440060(iVar5,0x1f,iVar3);
  return 1;
}
