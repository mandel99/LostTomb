#include "restore_types.h"
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799aa8 (*(undefined4 *)0x00799aa8u)
#define DAT_00799aac (*(undefined4 *)0x00799aacu)
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
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f567ac (*(undefined *)0x00f567acu)
extern void FUN_0040f860();
extern undefined4 FUN_0043f410();
extern void FUN_00440060();
extern void FUN_004405c0();
extern uint FUN_00441130();
extern void FUN_004413b0();
extern int FUN_0046a400();
extern void FUN_0047bf50();
extern undefined4 FUN_00485100();
extern void FUN_00485830();

undefined4 FUN_00440850(int param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  int local_4;
  
  iVar5 = param_5;
  iVar3 = param_4;
  iVar6 = DAT_00bca2d8 + param_5 * 0xe4 + param_4;
  FUN_004413b0(param_4,param_5,0x1e);
  iVar2 = FUN_0043f410(0x1e);
  if (iVar2 == 0) {
    return 0;
  }
  iVar2 = FUN_00441130(iVar3,iVar5,&param_4,&param_5,0x1e,0x1e);
  if (iVar2 == -1) {
    return 0;
  }
  iVar3 = FUN_00441130(iVar3,iVar5,&local_c,&local_10,0x1e,0x1f);
  if (iVar3 == -1) {
    return 0;
  }
  if (param_1 == 0) {
    FUN_0040f860(&DAT_009de62c,&DAT_00af598c,0x32c40);
    FUN_0040f860(&DAT_009d1b1c,&DAT_00ae8e7c,0xcb10);
    FUN_0040f860(&DAT_009b84fc,&DAT_00b285cc,0xcb10);
    FUN_0040f860(&DAT_009ab9ec,&DAT_00b67d1c,0xcb10);
    FUN_00485830(0);
    DAT_00bc971c = 1;
    FUN_004405c0(iVar2,&local_8,&local_14);
    iVar3 = FUN_0046a400(DAT_00eaac28,0x1e,param_4,param_5,0,0);
    (&DAT_00799ab0)[iVar3 * 0x42] = 1;
    (&DAT_00799aac)[iVar3 * 0x42] = iVar2;
    (&DAT_00799aa8)[iVar3 * 0x42] = iVar6;
    local_4 = iVar3;
    FUN_0047bf50(iVar3,param_4,param_5,1,1,local_8,8);
    FUN_00485100(iVar3);
    if (iVar2 == 0) {
      iVar4 = FUN_0046a400(DAT_00eaac28,0x1e,param_4,param_5 + 1,0,0);
      iVar5 = param_4;
      iVar7 = param_5 + 1;
    }
    else {
      iVar4 = FUN_0046a400(DAT_00eaac28,0x1e,param_4 + 1,param_5,0,0);
      iVar5 = param_4 + 1;
      iVar7 = param_5;
    }
    FUN_0047bf50(iVar4,iVar5,iVar7,1,1,local_14,8);
    (&DAT_00799aa8)[iVar4 * 0x42] = iVar6;
    (&DAT_00799aac)[iVar3 * 0x42] = iVar2;
    (&DAT_00799ab0)[iVar4 * 0x42] = 0;
    FUN_00485100(iVar4);
    iVar5 = FUN_0046a400(DAT_00eaac28,0x1f,local_c,local_10,0,0);
    uVar1 = DAT_00f567ac;
    (&DAT_00799ab0)[iVar5 * 0x42] = 0;
    (&DAT_00799aa8)[iVar5 * 0x42] = iVar6;
    FUN_0047bf50(iVar5,local_c,local_10,1,1,uVar1,8);
    FUN_00485100(iVar5);
    RESTORE_SLICE2(param_1,0) = (undefined2)iVar5;
    *(short *)(&DAT_00799a12 + iVar3 * 0x108) = (short)iVar4;
    *(short *)(&DAT_00799a10 + iVar4 * 0x108) = (short)local_4;
    *(undefined2 *)(&DAT_00799a12 + iVar4 * 0x108) = (undefined2)param_1;
    *(short *)(&DAT_00799a10 + iVar5 * 0x108) = (short)iVar4;
    FUN_00440060(iVar6,0x1e,local_4);
  }
  return 1;
}
