#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_0056f898 (*(undefined4 *)0x0056f898u)
#define DAT_0056f89c (*(undefined4 *)0x0056f89cu)
#define DAT_0056fd18 (*(undefined4 *)0x0056fd18u)
#define DAT_0056fd1c (*(undefined4 *)0x0056fd1cu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005e4efc (*(undefined4 *)0x005e4efcu)
#define DAT_005e657c (*(undefined4 *)0x005e657cu)
#define DAT_005e6970 (*(undefined *)0x005e6970u)
#define DAT_005f5c40 (*(undefined4 *)0x005f5c40u)
#define DAT_005f5c54 (*(undefined4 *)0x005f5c54u)
#define DAT_005f5c68 (*(undefined4 *)0x005f5c68u)
#define DAT_005f5c7c (*(undefined4 *)0x005f5c7cu)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a36 (*(undefined4 *)0x00799a36u)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a3a (*(undefined4 *)0x00799a3au)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
extern int FUN_0046a400();
extern undefined4 FUN_00485100();
extern void FUN_004ee6c0();
extern void FUN_004f3c90();
extern int FUN_004f4f80();
extern void FUN_004f5f10();

void FUN_004ee780(undefined4 param_1,int param_2,int param_3,undefined4 param_4,int param_5)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  undefined1 uVar6;
  int iVar7;
  int iVar8;
  
  iVar8 = param_5;
  iVar3 = DAT_005e6970;
  iVar7 = DAT_005e4ef4;
  iVar2 = param_5 * 0x108;
  *(undefined2 *)(&DAT_00799a10 + iVar2) = 0;
  *(char *)(&DAT_00799a2c + param_5 * 0x84) = (char)iVar3;
  *(undefined1 *)((int)&DAT_00799a2c + iVar2 + 1) = 2;
  *(undefined1 *)(&DAT_00799a2e + param_5 * 0x84) = 0;
  (&DAT_00799a3c)[param_5 * 0x84] = 0;
  *(undefined1 *)((int)&DAT_00799a2e + iVar2 + 1) = 0;
  if (iVar7 == 5) {
    uVar6 = (&DAT_005e657c)[iVar3 * 0x120];
  }
  else {
    uVar6 = (&DAT_005e4efc)[iVar3 * 0x480];
  }
  *(undefined1 *)(&DAT_00799a30 + param_5 * 0x42) = uVar6;
  *(undefined1 *)((int)&DAT_00799a30 + iVar2 + 1) = 0;
  *(undefined1 *)((int)&DAT_00799a30 + iVar2 + 2) = 0;
  *(undefined1 *)((int)&DAT_00799a30 + iVar2 + 3) = 0xf;
  *(undefined2 *)(&DAT_00799a34 + iVar2) = 0;
  *(undefined2 *)(&DAT_00799a36 + iVar2) = 0;
  *(undefined2 *)(&DAT_00799a38 + iVar2) = 0;
  *(undefined2 *)(&DAT_00799a3a + iVar2) = 0;
  *(undefined4 *)(&DAT_00799a40 + param_5 * 0x84) = 0;
  switch((int)(iVar7)) {
  case 1: ;
    uVar5 = (undefined2)*(undefined4 *)(&DAT_005f5c68 + iVar3 * 4);
    break;
  case 2: ;
  case 4: ;
    uVar5 = (undefined2)*(undefined4 *)(&DAT_005f5c40 + iVar3 * 4);
    break;
  case 3: ;
    uVar5 = (undefined2)*(undefined4 *)(&DAT_005f5c54 + iVar3 * 4);
    break;
  case 5: ;
    uVar5 = (undefined2)*(undefined4 *)(&DAT_005f5c7c + iVar3 * 4);
    break;
default: ;
    uVar5 = 0;
  }
  *(undefined2 *)(&DAT_00799aa0 + param_5 * 0x42) = uVar5;
  (&DAT_007999ea)[param_5 * 0x84] = 0;
  (&DAT_007999db)[iVar2] = 2;
  FUN_004ee6c0(param_5);
  FUN_004f4f80(param_5);
  iVar7 = 1;
  param_5 = 0;
  while( true ) {
    if (DAT_005e4ef4 == 5) {
      iVar3 = *(int *)(&DAT_0056f7ec + DAT_005e6970 * 4);
    }
    else {
      iVar3 = *(int *)(&DAT_0056f7d8 + DAT_005e6970 * 4);
    }
    if (iVar3 <= iVar7) break;
    if (DAT_005e4ef4 == 5) {
      iVar3 = *(int *)((int)&DAT_0056fd18 + param_5);
      iVar2 = *(int *)((int)&DAT_0056fd1c + param_5);
    }
    else {
      iVar3 = *(int *)((int)&DAT_0056f898 + param_5);
      iVar2 = *(int *)((int)&DAT_0056f89c + param_5);
    }
    iVar2 = FUN_0046a400(param_1,0xb7,iVar3 + param_2,iVar2 + param_3,0,iVar8 + 1);
    FUN_00485100(iVar2);
    iVar3 = DAT_005e4ef4;
    iVar4 = iVar2 * 0x108;
    *(short *)(&DAT_00799a10 + iVar4) = (short)iVar8;
    *(short *)(&DAT_00799a12 + iVar8 * 0x108) = (short)iVar2;
    iVar8 = DAT_005e6970;
    *(undefined2 *)(&DAT_00799a12 + iVar4) = 0;
    (&DAT_007999ea)[iVar2 * 0x84] = (short)iVar7;
    (&DAT_007999db)[iVar4] = 2;
    *(char *)(&DAT_00799a2c + iVar2 * 0x84) = (char)iVar8;
    *(undefined1 *)((int)&DAT_00799a2c + iVar4 + 1) = 2;
    *(undefined1 *)(&DAT_00799a2e + iVar2 * 0x84) = 0;
    (&DAT_00799a3c)[iVar2 * 0x84] = 0;
    *(undefined1 *)((int)&DAT_00799a2e + iVar4 + 1) = 0;
    if (iVar3 == 5) {
      uVar6 = (&DAT_005e657c)[(iVar7 + iVar8 * 0x24) * 8];
    }
    else {
      uVar6 = (&DAT_005e4efc)[(iVar8 * 0x90 + iVar7) * 8];
    }
    *(undefined1 *)(&DAT_00799a30 + iVar2 * 0x42) = uVar6;
    *(undefined1 *)((int)&DAT_00799a30 + iVar4 + 1) = 0;
    *(undefined1 *)((int)&DAT_00799a30 + iVar4 + 2) = 0;
    *(undefined1 *)((int)&DAT_00799a30 + iVar4 + 3) = 0xf;
    *(undefined2 *)(&DAT_00799a34 + iVar4) = 0;
    *(undefined2 *)(&DAT_00799a36 + iVar4) = 0;
    *(undefined2 *)(&DAT_00799a38 + iVar4) = 0;
    *(undefined2 *)(&DAT_00799a3a + iVar4) = 0;
    *(undefined4 *)(&DAT_00799a40 + iVar2 * 0x84) = 0;
    FUN_004ee6c0(iVar2);
    iVar7 = iVar7 + 1;
    param_5 = param_5 + 8;
    iVar8 = iVar2;
  }
  sVar1 = (&DAT_007999ea)[iVar8 * 0x84];
  FUN_004f3c90(iVar8 - sVar1);
  FUN_004f5f10(iVar8 - sVar1);
  return;
}
