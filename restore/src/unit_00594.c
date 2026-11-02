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
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f5659c (*(undefined *)0x00f5659cu)
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

undefined4 FUN_00440b10(int param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar3 = param_4;
  if (DAT_00bca2d0 <= param_4) {
    iVar3 = DAT_00bca2d0 + -1;
  }
  iVar4 = param_5;
  if (DAT_00bca2d4 <= param_5) {
    iVar4 = DAT_00bca2d4 + -1;
  }
  iVar7 = DAT_00bca2d8 + iVar4 * 0xe4 + iVar3;
  FUN_004413b0(iVar3,iVar4,0x21);
  iVar2 = FUN_0043f410(0x21);
  if (iVar2 == 0) {
    return 0;
  }
  iVar2 = FUN_00441130(iVar3,iVar4,&local_20,&local_24,0x21,0x21);
  if (iVar2 == -1) {
    return 0;
  }
  iVar2 = FUN_00441130(iVar3,iVar4,&param_5,&param_4,0x21,0x1e);
  if (iVar2 == -1) {
    return 0;
  }
  iVar3 = FUN_00441130(iVar3,iVar4,&local_18,&local_1c,0x21,0x1f);
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
  iVar3 = FUN_0046a400(DAT_00eaac28,0x21,local_20,local_24,0,0);
  uVar1 = DAT_00f5659c;
  (&DAT_00799ab0)[iVar3 * 0x42] = 1;
  (&DAT_00799aa8)[iVar3 * 0x42] = iVar7;
  local_8 = iVar3;
  FUN_0047bf50(iVar3,local_20,local_24,2,2,uVar1,8);
  FUN_00485100(iVar3);
  iVar4 = FUN_0046a400(DAT_00eaac28,0x1f,local_18,local_1c,0,0);
  uVar1 = DAT_00f567ac;
  (&DAT_00799ab0)[iVar4 * 0x42] = 0;
  (&DAT_00799aa8)[iVar4 * 0x42] = iVar7;
  local_c = iVar4;
  FUN_0047bf50(iVar4,local_18,local_1c,1,1,uVar1,8);
  FUN_00485100(iVar4);
  FUN_004405c0(iVar2,&local_10,&local_14);
  iVar5 = FUN_0046a400(DAT_00eaac28,0x1e,param_5,param_4,0,0);
  (&DAT_00799ab0)[iVar5 * 0x42] = 0;
  (&DAT_00799aac)[iVar5 * 0x42] = iVar2;
  (&DAT_00799aa8)[iVar5 * 0x42] = iVar7;
  local_4 = iVar5;
  FUN_0047bf50(iVar5,param_5,param_4,1,1,local_10,8);
  FUN_00485100(iVar5);
  if (iVar2 == 0) {
    iVar6 = FUN_0046a400(DAT_00eaac28,0x1e,param_5,param_4 + 1,0,0);
    FUN_0047bf50(iVar6,param_5,param_4 + 1,1,1,local_14,8);
  }
  else {
    iVar6 = FUN_0046a400(DAT_00eaac28,0x1e,param_5 + 1,param_4,0,0);
    FUN_0047bf50(iVar6,param_5 + 1,param_4,1,1,local_14,8);
  }
  (&DAT_00799ab0)[iVar6 * 0x42] = 0;
  (&DAT_00799aa8)[iVar6 * 0x42] = iVar7;
  (&DAT_00799aac)[iVar5 * 0x42] = iVar2;
  (&DAT_00799aac)[iVar4 * 0x42] = iVar2;
  (&DAT_00799aac)[iVar3 * 0x42] = iVar2;
  FUN_00485100(iVar6);
  *(short *)(&DAT_00799a12 + iVar3 * 0x108) = (short)local_c;
  *(short *)(&DAT_00799a10 + iVar4 * 0x108) = (short)local_8;
  *(short *)(&DAT_00799a12 + iVar4 * 0x108) = (short)local_4;
  *(short *)(&DAT_00799a10 + iVar5 * 0x108) = (short)local_c;
  RESTORE_SLICE2(param_1,0) = (undefined2)iVar6;
  *(undefined2 *)(&DAT_00799a12 + iVar5 * 0x108) = (undefined2)param_1;
  *(short *)(&DAT_00799a10 + iVar6 * 0x108) = (short)local_4;
  FUN_00440060(iVar7,0x21,local_8);
  return 1;
}
