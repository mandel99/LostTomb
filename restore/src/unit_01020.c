#include "restore_types.h"
#include "sprite_libraries.h"
#define DAT_005d06e4 (*(undefined4 *)0x005d06e4u)
#define DAT_006059d4 (*(undefined *)0x006059d4u)
#define DAT_006059d8 (*(undefined *)0x006059d8u)
#define DAT_006059dc (*(undefined *)0x006059dcu)
#define DAT_006059e0 (*(undefined *)0x006059e0u)
#define DAT_006059e4 (*(undefined *)0x006059e4u)
#define DAT_006059e8 (*(undefined *)0x006059e8u)
#define DAT_006fdce4 (*(undefined *)0x006fdce4u)
#define DAT_006fdce8 (*(undefined *)0x006fdce8u)
#define DAT_006fdcec (*(char *)0x006fdcecu)
#define DAT_006fdcf0 (*(undefined *)0x006fdcf0u)
#define DAT_006fdcf4 (*(undefined *)0x006fdcf4u)
#define DAT_006fdcf8 (*(char *)0x006fdcf8u)
#define DAT_006fdcfc (*(undefined *)0x006fdcfcu)
#define DAT_006fdd00 (*(undefined *)0x006fdd00u)
#define DAT_006fdd04 (*(char *)0x006fdd04u)
#define DAT_00705c1c (*(undefined *)0x00705c1cu)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a28 (*(undefined4 *)0x00799a28u)
#define DAT_00799a5f (*(undefined4 *)0x00799a5fu)
#define DAT_00799a80 (*(undefined4 *)0x00799a80u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00799a9c (*(undefined4 *)0x00799a9cu)
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_00799aa8 (*(undefined4 *)0x00799aa8u)
#define DAT_00799ab0 (*(undefined4 *)0x00799ab0u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc96d8 (*(undefined *)0x00bc96d8u)
#define DAT_00bc96dc (*(undefined *)0x00bc96dcu)
#define DAT_00bc9800 (*(undefined *)0x00bc9800u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca310 (*(undefined *)0x00bca310u)
#define DAT_00bcb9b0 (*(undefined *)0x00bcb9b0u)
#define DAT_00bcb9ca (*(undefined4 *)0x00bcb9cau)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcba42 (*(undefined4 *)0x00bcba42u)
#define DAT_00ea3ff8 (*(undefined4 *)0x00ea3ff8u)
#define DAT_00ea3ffa (*(undefined4 *)0x00ea3ffau)
#define DAT_00ea3ffc (*(undefined4 *)0x00ea3ffcu)
#define DAT_00ea4000 (*(undefined4 *)0x00ea4000u)
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00ea5cf6 (*(undefined4 *)0x00ea5cf6u)
#define DAT_00ea5d14 (*(undefined4 *)0x00ea5d14u)
#define DAT_00ea5d18 (*(undefined4 *)0x00ea5d18u)
#define DAT_00ea5d1c (*(undefined4 *)0x00ea5d1cu)
#define DAT_00ea5e64 (*(undefined4 *)0x00ea5e64u)
#define DAT_00ea5e68 (*(undefined4 *)0x00ea5e68u)
#define DAT_00ea5e6c (*(undefined4 *)0x00ea5e6cu)
#define DAT_00ea5e70 (*(undefined4 *)0x00ea5e70u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00ea6082 (*(undefined4 *)0x00ea6082u)
#define DAT_00ea620c (*(undefined4 *)0x00ea620cu)
#define DAT_00ea6240 (*(undefined4 *)0x00ea6240u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
#define DAT_00f564a4 (*(undefined *)0x00f564a4u)
#define DAT_00f564ac (*(undefined *)0x00f564acu)
#define DAT_00f564b4 (*(undefined *)0x00f564b4u)
#define DAT_00f564bc (*(undefined *)0x00f564bcu)
#define DAT_00f564c4 (*(undefined *)0x00f564c4u)
#define DAT_00f564cc (*(undefined *)0x00f564ccu)
#define DAT_00f564d4 (*(undefined *)0x00f564d4u)
#define DAT_00f564e4 (*(undefined *)0x00f564e4u)
#define DAT_00f56554 (*(undefined *)0x00f56554u)
#define DAT_00f5655c (*(undefined *)0x00f5655cu)
#define DAT_00f5656c (*(undefined *)0x00f5656cu)
#define DAT_00f56574 (*(undefined *)0x00f56574u)
#define DAT_00f5657c (*(undefined *)0x00f5657cu)
#define DAT_00f5658c (*(undefined *)0x00f5658cu)
#define DAT_00f56594 (*(undefined *)0x00f56594u)
#define DAT_00f5659c (*(undefined *)0x00f5659cu)
#define DAT_00f565b4 (*(undefined *)0x00f565b4u)
#define DAT_00f565bc (*(undefined *)0x00f565bcu)
#define DAT_00f565dc (*(undefined *)0x00f565dcu)
#define DAT_00f565e4 (*(undefined *)0x00f565e4u)
#define DAT_00f5660c (*(undefined *)0x00f5660cu)
#define DAT_00f56614 (*(undefined *)0x00f56614u)
#define DAT_00f5661c (*(undefined *)0x00f5661cu)
#define DAT_00f56624 (*(undefined *)0x00f56624u)
#define DAT_00f5662c (*(undefined *)0x00f5662cu)
#define DAT_00f56634 (*(undefined *)0x00f56634u)
#define DAT_00f5663c (*(undefined *)0x00f5663cu)
#define DAT_00f56644 (*(undefined *)0x00f56644u)
#define DAT_00f5664c (*(undefined *)0x00f5664cu)
#define DAT_00f5667c (*(undefined *)0x00f5667cu)
#define DAT_00f56684 (*(undefined *)0x00f56684u)
#define DAT_00f5668c (*(undefined *)0x00f5668cu)
#define DAT_00f566a4 (*(undefined *)0x00f566a4u)
#define DAT_00f566bc (*(undefined *)0x00f566bcu)
#define DAT_00f566c4 (*(undefined *)0x00f566c4u)
#define DAT_00f566cc (*(undefined *)0x00f566ccu)
#define DAT_00f566d4 (*(undefined *)0x00f566d4u)
#define DAT_00f566dc (*(undefined *)0x00f566dcu)
#define DAT_00f566f4 (*(undefined *)0x00f566f4u)
#define DAT_00f5672c (*(undefined *)0x00f5672cu)
#define DAT_00f56734 (*(undefined *)0x00f56734u)
#define DAT_00f567ac (*(undefined *)0x00f567acu)
#define DAT_00f567bc (*(undefined *)0x00f567bcu)
#define DAT_00f567d4 (*(undefined *)0x00f567d4u)
#define DAT_00f567dc (*(undefined *)0x00f567dcu)
#define DAT_00f567ec (*(undefined *)0x00f567ecu)
#define DAT_00f567f4 (*(undefined *)0x00f567f4u)
#define DAT_00f567fc (*(undefined *)0x00f567fcu)
#define DAT_00f56804 (*(undefined *)0x00f56804u)
#define DAT_00f56854 (*(undefined *)0x00f56854u)
#define DAT_00f5694c (*(undefined *)0x00f5694cu)
#define DAT_00f56984 (*(undefined *)0x00f56984u)
#define DAT_00f56994 (*(undefined *)0x00f56994u)
#define DAT_00f5699c (*(undefined *)0x00f5699cu)
#define DAT_00f569d4 (*(undefined *)0x00f569d4u)
#define DAT_00f569dc (*(undefined *)0x00f569dcu)
#define DAT_00f569e4 (*(undefined *)0x00f569e4u)
#define DAT_00f569f4 (*(undefined *)0x00f569f4u)
#define DAT_00f569fc (*(undefined *)0x00f569fcu)
#define DAT_00f56a3c (*(undefined *)0x00f56a3cu)
#define DAT_00f56a44 (*(undefined *)0x00f56a44u)
#define DAT_00f56b3c (*(undefined *)0x00f56b3cu)
#define DAT_00f56bc4 (*(undefined *)0x00f56bc4u)
#define DAT_00f56bcc (*(undefined *)0x00f56bccu)
#define DAT_00f56bf4 (*(undefined *)0x00f56bf4u)
#define DAT_00f56c0c (*(undefined *)0x00f56c0cu)
#define DAT_00f56c1c (*(undefined *)0x00f56c1cu)
#define DAT_00f56c54 (*(undefined *)0x00f56c54u)
#define DAT_00f56c5c (*(undefined *)0x00f56c5cu)
#define DAT_00f56cc4 (*(undefined *)0x00f56cc4u)
#define DAT_00f56d54 (*(undefined *)0x00f56d54u)
#define DAT_00f56d5c (*(undefined *)0x00f56d5cu)
#define DAT_00f56d64 (*(undefined *)0x00f56d64u)
#define DAT_00f57204 (*(undefined *)0x00f57204u)
#define DAT_00f57214 (*(undefined *)0x00f57214u)
#define DAT_00f5724c (*(undefined *)0x00f5724cu)
#define DAT_00f57254 (*(undefined *)0x00f57254u)
#define DAT_00f5725c (*(undefined *)0x00f5725cu)
#define DAT_00f5726c (*(undefined *)0x00f5726cu)
#define DAT_00f57274 (*(undefined *)0x00f57274u)
#define DAT_00f5727c (*(undefined *)0x00f5727cu)
#define DAT_00f572b4 (*(undefined *)0x00f572b4u)
#define DAT_00f572bc (*(undefined *)0x00f572bcu)
#define DAT_00f572dc (*(undefined *)0x00f572dcu)
#define DAT_00f572fc (*(undefined *)0x00f572fcu)
#define DAT_00f57304 (*(undefined *)0x00f57304u)
#define DAT_00f5730c (*(undefined *)0x00f5730cu)
#define DAT_00f57374 (*(undefined *)0x00f57374u)
#define DAT_00f5737c (*(undefined *)0x00f5737cu)
#define DAT_00f57384 (*(undefined *)0x00f57384u)
#define PTR_s_TEMPLE_NILE_005e018c (*(unsigned char * *)0x005e018cu)
#define _DAT_006fdce8 (*(undefined4 *)0x006fdce8u)
#define _DAT_006fdcf4 (*(undefined4 *)0x006fdcf4u)
#define _DAT_006fdd00 (*(undefined4 *)0x006fdd00u)
extern void FUN_004015a0();
extern void FUN_004017a0();
extern bool FUN_00405ba0();
extern void FUN_0040da90();
extern void FUN_0040dca0();
extern void FUN_00417180();
extern void FUN_0041ff90();
extern void FUN_0041ffc0();
extern void FUN_004206a0();
extern void FUN_004209e0();
extern void FUN_00420d10();
extern void FUN_00421010();
extern void FUN_004212f0();
extern void FUN_00440060();
extern void FUN_00440540();
extern undefined4 FUN_00451bc0();
extern undefined4 FUN_0045b490();
extern int FUN_0046a400();
extern int FUN_0046b100();
extern void FUN_0046cea0();
extern void FUN_0046cf40();
extern int FUN_0046efd0();
extern void FUN_00472790();
extern int FUN_00475b50();
extern uint32_t FUN_0047a1c0(int32_t);
extern void FUN_0047afd0();
extern void FUN_0047bf50();
extern void FUN_0047c2b0();
extern int FUN_0047cc70();
extern void FUN_0047cf00();
extern int FUN_0047d060();
extern void FUN_0047d1f0();
extern undefined4 FUN_0047d350();
extern void FUN_00482580();
extern undefined4 FUN_00485100();
extern void FUN_00487ec0();
extern void FUN_00488e60();
extern int FUN_004b6e10();
extern void FUN_004bfb30();
extern void FUN_004c0d50();
extern void FUN_004c3420();
extern undefined4 FUN_004c5120();
extern void FUN_004df9d0();
extern void FUN_004dfbb0();
extern undefined2 FUN_004e57f0();
extern void FUN_004e5840();
extern void FUN_004e6840();
extern void FUN_004e69c0();
extern void FUN_004ee4a0();
extern void FUN_004ee780();
extern int FUN_004f4f80();
extern undefined4 FUN_004f6520();
extern void FUN_0053d850();
extern void FUN_005417b0();
extern void FUN_00541920();
extern void FUN_00545c30();
extern void FUN_005468d0();
extern void FUN_0054fba0();
extern void FUN_00556480();
extern void FUN_00559130();
extern void FUN_005596d0();
extern uint FUN_00562348();

void FUN_00470990(undefined4 param_1,int param_2,int param_3,int param_4,int param_5,
                 undefined4 param_6,int param_7,int param_8)

{
  short sVar1;
  byte bVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined1 uVar7;
  char cVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  uint uVar13;
  undefined2 uVar14;
  undefined2 uVar15;
  int *piVar16;
  bool bVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  int local_64 [25];
  
  uVar6 = param_8;
  iVar5 = DAT_00f569d4;
  uVar4 = DAT_00f5694c;
  iVar11 = DAT_00f566d4;
  iVar12 = DAT_00f566cc;
  iVar10 = DAT_00f566c4;
  uVar18 = DAT_00f56594;
  uVar3 = DAT_00f56554;
  uVar19 = DAT_00f564ac;
  iVar9 = DAT_00eaac28;
  if (param_2 == 0x39) {
    if ((&DAT_007999ea)[param_3 * 0x84] == 0xd) {
      param_2 = 0x28;
    }
    else {
      param_2 = ((&DAT_007999ea)[param_3 * 0x84] != 0xb) + 0x2c;
    }
  }
  RESTORE_SLICE2(param_8,0) = (undefined2)param_3;
  uVar14 = (undefined2)param_4;
  uVar15 = (undefined2)param_5;
  switch((int)(param_2)) {
  case 10: ;
    if (((&DAT_00799a5f)[param_3 * 0x108] == '\0') && ((&DAT_007999f4)[param_3 * 0x84] == 0)) {
      FUN_0046cf40(param_3);
      return;
    }
    goto LAB_00470a4a;
  case 0xb: ;
  case 0xc: ;
  case 0xd: ;
  case 0xe: ;
  case 0xf: ;
  case 0x10: ;
  case 0x11: ;
  case 0x12: ;
  case 0x13: ;
  case 0x14: ;
  case 0x15: ;
  case 0x16: ;
  case 0x17: ;
  case 0x18: ;
  case 0x19: ;
  case 0x1a: ;
  case 0x1b: ;
  case 0x1c: ;
  case 0x1d: ;
    if (((&DAT_00799a5f)[param_3 * 0x108] == '\0') && ((&DAT_007999f4)[param_3 * 0x84] == 0)) {
      FUN_0046cf40(param_3);
      return;
    }
LAB_00470a4a: ;
    FUN_0046cea0(param_3,param_2);
    return;
  case 0x1e: ;
    if ((&DAT_007999e8)[*(short *)(&DAT_00799a12 + param_3 * 0x108) * 0x84] == 0x1e) {
      FUN_00440540(param_3,param_4,param_5);
    }
    iVar11 = FUN_00475b50(param_3);
    if (iVar11 != param_3) {
      return;
    }
    if ((&DAT_00799ab0)[param_3 * 0x42] == 0) {
      return;
    }
    FUN_00440060((&DAT_00799aa8)[param_3 * 0x42],0x1e,param_3);
    return;
  case 0x1f: ;
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,DAT_00f567ac,8);
    iVar11 = FUN_00475b50(param_3);
    if (iVar11 != param_3) {
      return;
    }
    if ((&DAT_00799ab0)[param_3 * 0x42] == 0) {
      return;
    }
    FUN_00440060((&DAT_00799aa8)[param_3 * 0x42],0x1f,param_3);
    return;
  case 0x20: ;
  case 0x25: ;
    iVar11 = DAT_00f564a4;
    break;
  case 0x21: ;
    FUN_0047bf50(param_3,param_4,param_5,2,2,DAT_00f5659c,8);
    iVar11 = FUN_00475b50(param_3);
    if (iVar11 != param_3) {
      return;
    }
    if ((&DAT_00799ab0)[param_3 * 0x42] == 0) {
      return;
    }
    FUN_00440060((&DAT_00799aa8)[param_3 * 0x42],0x21,param_3);
    return;
  case 0x22: ;
    iVar11 = DAT_00f565b4;
    break;
  case 0x23: ;
    iVar11 = DAT_00f565bc;
    break;
  case 0x24: ;
    iVar11 = DAT_00f5658c;
    break;
default: ;
    goto switchD_004709e5_caseD_26;
  case 0x28: ;
  case 0x2c: ;
  case 0x2d: ;
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,DAT_00f5662c,8);
    if (param_7 == 0) {
      iVar11 = (int)*(short *)(&DAT_00799a12 + param_3 * 0x108);
    }
    else {
      iVar9 = FUN_004b6e10(param_3);
      iVar10 = param_3 * 0x108;
      *(undefined2 *)(&DAT_00799a10 + iVar10) = 0;
      *(short *)(&DAT_00799a28 + iVar10) = (short)iVar9;
      if (param_2 == 0x28) {
        (&DAT_007999ea)[param_3 * 0x84] = 0xd;
        iVar11 = iVar9 * 0x90;
        bVar17 = (char)DAT_006fdd04 != '\0';
        *(undefined2 *)(&DAT_00bcb9cc + iVar11) = 0xd;
        if (bVar17) {
          (&DAT_00bcb9ca)[iVar11] = (undefined1)DAT_006fdcfc;
          FUN_00472790(DAT_006fdd04 & 0xff,iVar9,param_4 + 4,param_5 + 1);
          *(undefined2 *)(&DAT_00bcba42 + iVar11) = DAT_006fdd00;
          _DAT_006fdd00 = 0xffffffff;
          DAT_006fdd04 = DAT_006fdd04 & 0xffffff00;
          RESTORE_SLICE1(DAT_006fdcfc,0) = 0xff;
        }
      }
      else if (param_2 == 0x2c) {
        (&DAT_007999ea)[param_3 * 0x84] = 0xb;
        iVar11 = iVar9 * 0x90;
        bVar17 = (char)DAT_006fdcf8 != '\0';
        *(undefined2 *)(&DAT_00bcb9cc + iVar11) = 0xb;
        if (bVar17) {
          (&DAT_00bcb9ca)[iVar11] = (undefined1)DAT_006fdcf0;
          FUN_00472790(DAT_006fdcf8 & 0xff,iVar9,param_4 + 4,param_5 + 1);
          *(undefined2 *)(&DAT_00bcba42 + iVar11) = DAT_006fdcf4;
          _DAT_006fdcf4 = 0xffffffff;
          DAT_006fdcf8 = DAT_006fdcf8 & 0xffffff00;
          RESTORE_SLICE1(DAT_006fdcf0,0) = 0xff;
        }
      }
      else {
        (&DAT_007999ea)[param_3 * 0x84] = 0xc;
        iVar11 = iVar9 * 0x90;
        bVar17 = (char)DAT_006fdcec != '\0';
        *(undefined2 *)(&DAT_00bcb9cc + iVar11) = 0xc;
        if (bVar17) {
          (&DAT_00bcb9ca)[iVar11] = (undefined1)DAT_006fdce4;
          FUN_00472790(DAT_006fdcec & 0xff,iVar9,param_4 + 4,param_5 + 1);
          *(undefined2 *)(&DAT_00bcba42 + iVar11) = DAT_006fdce8;
          _DAT_006fdce8 = 0xffffffff;
          DAT_006fdcec = DAT_006fdcec & 0xffffff00;
          RESTORE_SLICE1(DAT_006fdce4,0) = 0xff;
        }
      }
      iVar11 = FUN_0046a400(param_1,0x36,param_4 + 3,param_5 + -1,0,0);
      FUN_00485100(iVar11);
      iVar12 = iVar11 * 0x108;
      *(short *)(&DAT_00799a28 + iVar12) = (short)iVar9;
      *(undefined2 *)(&DAT_00799a10 + iVar12) = (undefined2)param_8;
      *(short *)(&DAT_00799a12 + iVar10) = (short)iVar11;
      *(undefined2 *)(&DAT_00799a12 + iVar12) = 0;
    }
    FUN_0047bf50(iVar11,param_4 + 3,param_5 + -1,4,4,DAT_00f5662c + 1,8);
    return;
  case 0x29: ;
  case 0x2a: ;
  case 0x2b: ;
    iVar9 = (*(byte *)(&DAT_00799a82 + param_3 * 0x84) & 0xf) - DAT_00bca2e0 / 2;
    bVar2 = *(byte *)(&DAT_00799a82 + param_3 * 0x84) >> 4;
    uVar13 = (uint)bVar2;
    iVar11 = iVar9 + -1;
    if (iVar11 < 0) {
      iVar11 = iVar9 + 3;
    }
    if (bVar2 < 2) {
      iVar11 = (&DAT_00f55bb4)[*(int *)(&DAT_005d06e4 + param_2 * 0x14) * 2] + uVar13 * 4 + iVar11;
    }
    else if (param_2 == 0x29) {
      iVar11 = DAT_00f5730c + -8 + uVar13 * 4 + iVar11;
    }
    else if (param_2 == 0x2a) {
      iVar11 = DAT_00f57304 + -8 + uVar13 * 4 + iVar11;
    }
    else {
      iVar11 = DAT_00f572fc + -8 + uVar13 * 4 + iVar11;
    }
    break;
  case 0x2e: ;
    iVar11 = DAT_00f5663c;
    break;
  case 0x2f: ;
    iVar11 = DAT_00f56994;
    break;
  case 0x31: ;
    iVar11 = DAT_00f56634;
    break;
  case 0x32: ;
    (&DAT_00ea5e70)[DAT_00eaac28 * 0x1276] = 1;
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,uVar4,8);
    iVar11 = DAT_00eaac28;
    iVar9 = DAT_00eaac28 * 0x49d8;
    if ((&DAT_00ea5e68)[DAT_00eaac28 * 0x1276] != 0) {
      return;
    }
    (&DAT_00ea5e6c)[DAT_00eaac28 * 0x1276] = param_3;
    *(undefined2 *)(&DAT_00ea5e64 + iVar11 * 0x1276) = uVar14;
    *(undefined2 *)((int)&DAT_00ea5e64 + iVar9 + 2) = uVar15;
    (&DAT_00ea5e68)[iVar11 * 0x1276] = DAT_00bca2d8 + param_5 * 0xe4 + param_4;
    return;
  case 0x33: ;
    iVar11 = DAT_00f5656c;
    break;
  case 0x35: ;
    iVar11 = DAT_00f56574;
    break;
  case 0x37: ;
    iVar11 = DAT_00f5661c;
    break;
  case 0x3c: ;
  case 0x97: ;
  case 0x98: ;
  case 0x99: ;
  case 0x9a: ;
  case 0x9b: ;
    iVar11 = DAT_00f564e4;
    break;
  case 0x3d: ;
    iVar11 = DAT_00f564b4;
    break;
  case 0x3e: ;
    iVar11 = DAT_00f564bc;
    break;
  case 0x3f: ;
    iVar11 = DAT_00f564c4;
    break;
  case 0x40: ;
    iVar11 = DAT_00f5667c;
    break;
  case 0x41: ;
  case 0x42: ;
  case 0x43: ;
  case 0x44: ;
  case 0x45: ;
  case 0x9c: ;
  case 0x9d: ;
  case 0x9e: ;
  case 0x9f: ;
  case 0xa0: ;
    uVar19 = 10;
    iVar11 = FUN_004c5120(param_3,10);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(&PTR_s_TEMPLE_NILE_005e018c)[iVar11],uVar19);
    FUN_004c3420(param_3,param_4,param_5,param_1,param_2,param_7);
    return;
  case 0x46: ;
    iVar10 = DAT_00f564cc;
    if (param_7 != 0) {
      FUN_005596d0();
      uVar7 = FUN_0046b100(param_3);
      *(undefined1 *)((int)&DAT_00799a98 + param_3 * 0x108 + 1) = uVar7;
      iVar10 = DAT_00f564cc;
    }
    goto LAB_00472378;
  case 0x47: ;
  case 0xbf: ;
    iVar11 = DAT_00f56734;
    if (param_7 != 0) {
      FUN_005596d0();
      uVar7 = FUN_0046b100(param_3);
      *(undefined1 *)((int)&DAT_00799a98 + param_3 * 0x108 + 1) = uVar7;
      iVar11 = DAT_00f56734;
    }
    break;
  case 0x48: ;
  case 0xc1: ;
    local_64[3] = 0x160;
    local_64[6] = 0x160;
    local_64[9] = 0x160;
    local_64[0xc] = 0x160;
    local_64[0xf] = 0x160;
    local_64[0x12] = 0x160;
    local_64[0x15] = 0x160;
    local_64[0x18] = 0x160;
    local_64[0] = 0x15f;
    local_64[1] = 1;
    local_64[2] = 0;
    local_64[4] = 2;
    local_64[5] = 0;
    local_64[7] = 0;
    local_64[8] = 1;
    local_64[10] = 1;
    local_64[0xb] = 1;
    local_64[0xd] = 2;
    local_64[0xe] = 1;
    local_64[0x10] = 0;
    local_64[0x11] = 2;
    local_64[0x13] = 1;
    local_64[0x14] = 2;
    local_64[0x16] = 2;
    local_64[0x17] = 2;
    param_8 = param_3;
    if (param_7 != 0) {
      uVar7 = FUN_0046b100(param_3);
      *(undefined1 *)((int)&DAT_00799a98 + param_3 * 0x108 + 1) = uVar7;
      *(undefined2 *)(&DAT_00799a10 + param_3 * 0x108) = 0;
    }
    param_2 = 0;
    piVar16 = local_64;
    do {
      iVar11 = (&DAT_00f55bb4)[*piVar16 * 2];
      if (((0 < param_2) && (param_7 == 0)) &&
         (iVar9 = (int)(short)(&DAT_007999ea)[param_3 * 0x84], iVar9 != 0)) {
        if (iVar9 < 0x1f) {
          iVar10 = DAT_00f566bc + -4 + iVar9 * 4;
        }
        else {
          iVar10 = DAT_00f57204 + -0x7c + iVar9 * 4;
        }
        iVar9 = FUN_0047d350(iVar9,0);
        iVar11 = FUN_0045b490((int)(short)(&DAT_00799a14)[param_3 * 0x84]);
        iVar11 = iVar10 + iVar9 + iVar11;
      }
      FUN_0047bf50(param_3,piVar16[-2] + param_4,piVar16[-1] + param_5,param_6,param_6,iVar11,8);
      if (param_2 < 8) {
        if (param_7 == 0) {
          param_3 = (int)*(short *)(&DAT_00799a12 + param_3 * 0x108);
          param_8 = param_3;
        }
        else {
          param_3 = FUN_0046a400(param_1,0x49,piVar16[1] + param_4,piVar16[2] + param_5,0,0);
          FUN_00485100(param_3);
          *(short *)(&DAT_00799a10 + param_3 * 0x108) = (short)param_8;
          *(short *)(&DAT_00799a12 + param_8 * 0x108) = (short)param_3;
          param_8 = param_3;
        }
      }
      piVar16 = piVar16 + 3;
      param_2 = param_2 + 1;
    } while (param_2 < 9);
    return;
  case 0x4a: ;
    FUN_0047d060(param_4,param_5,0);
    *(undefined1 *)(&DAT_00799a82 + param_3 * 0x84) = DAT_00bc96d8;
    cVar8 = FUN_0047a1c0(param_4);
    if (cVar8 == '\0') {
      FUN_0047d1f0(param_3,param_4,param_5,DAT_00f56c0c + DAT_00bc96dc);
      return;
    }
    FUN_0047d1f0(param_3,param_4,param_5,DAT_00f5724c + DAT_00bc96dc);
    return;
  case 0x4b: ;
  case 0xc0: ;
    FUN_0047d060(param_4,param_5,0);
    if (param_7 != 0) {
      DAT_00705c1c = DAT_00705c1c + 1;
    }
    *(undefined1 *)(&DAT_00799a82 + param_3 * 0x84) = DAT_00bc96d8;
    cVar8 = FUN_0047a1c0(param_4);
    if (cVar8 == '\0') {
      FUN_0047d1f0(param_3,param_4,param_5,DAT_00f56cc4 + DAT_00bc96dc);
      return;
    }
    FUN_0047d1f0(param_3,param_4,param_5,DAT_00f5727c + DAT_00bc96dc);
    return;
  case 0x4c: ;
    FUN_0047cc70(param_4,param_5,0,0);
    *(undefined1 *)(&DAT_00799a82 + param_3 * 0x84) = DAT_00bc96d8;
    cVar8 = FUN_0047a1c0(param_4);
    if (cVar8 == '\0') {
      FUN_0047cf00(param_3,param_4,param_5,DAT_00f56bcc + DAT_00bc96dc);
      return;
    }
    FUN_0047cf00(param_3,param_4,param_5,DAT_00f5726c + DAT_00bc96dc);
    return;
  case 0x4d: ;
    *(undefined4 *)(&DAT_00ea620c + DAT_00eaac28 * 0x49d8) = 1;
    goto LAB_00472378;
  case 0x4e: ;
    uVar19 = 8;
    *(undefined4 *)(&DAT_00ea620c + DAT_00eaac28 * 0x49d8) = 1;
    goto LAB_00472422;
  case 0x4f: ;
    *(undefined4 *)(&DAT_00ea620c + DAT_00eaac28 * 0x49d8) = 1;
    break;
  case 0x50: ;
    iVar11 = DAT_00f569dc;
    break;
  case 0x51: ;
    iVar11 = DAT_00f566a4;
    break;
  case 0x54: ;
  case 0x55: ;
  case 0xbb: ;
    (&DAT_00ea5cf6)[DAT_00eaac28 * 0x24ec] = 1;
    goto LAB_00472125;
  case 0x56: ;
    iVar11 = DAT_00f56614;
    break;
  case 0x58: ;
    uVar19 = 8;
    uVar13 = FUN_00562348(8);
    iVar12 = (uVar13 & 1) + iVar5;
    goto LAB_00472422;
  case 0x59: ;
    uVar19 = 8;
    iVar12 = DAT_00f569d4 + 2;
    goto LAB_00472422;
  case 0x5a: ;
    if (param_7 != 0) {
      FUN_0047cc70(param_4,param_5,0,1);
      *(undefined1 *)(&DAT_00799a82 + param_3 * 0x84) = DAT_00bc96d8;
    }
    FUN_00487ec0(param_3);
    return;
  case 0x5c: ;
    iVar11 = DAT_00f564d4;
    break;
  case 0x5d: ;
    uVar19 = 8;
    iVar12 = DAT_00f565dc;
    goto LAB_00472422;
  case 0x5e: ;
    *(undefined4 *)(&DAT_00ea6240 + DAT_00eaac28 * 0x49d8) = 1;
  case 0xb9: ;
  case 0xba: ;
    iVar11 = DAT_00f56984;
    break;
  case 0x5f: ;
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,DAT_00f5694c,8);
    iVar11 = DAT_00eaac28;
    iVar9 = DAT_00eaac28 * 0x49d8;
    if ((&DAT_00ea5d18)[DAT_00eaac28 * 0x1276] != 0) {
      return;
    }
    (&DAT_00ea5d1c)[DAT_00eaac28 * 0x1276] = param_3;
    *(undefined2 *)(&DAT_00ea5d14 + iVar11 * 0x1276) = uVar14;
    *(undefined2 *)((int)&DAT_00ea5d14 + iVar9 + 2) = uVar15;
    (&DAT_00ea5d18)[iVar11 * 0x1276] = DAT_00bca2d8 + param_5 * 0xe4 + param_4;
    return;
  case 99: ;
    iVar11 = param_3 * 0x108;
    if ((&DAT_007999d8)[iVar11] != '\x04') {
      uVar13 = (byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar11)] & 3;
      if ((*(char *)((int)&DAT_007999dc + iVar11 + 1) == '\0') ||
         (iVar9 = DAT_00f56c54, 1 < (short)(&DAT_007999ea)[param_3 * 0x84])) {
        if (2 < uVar13) {
          uVar13 = 3;
        }
        iVar9 = DAT_00f56c5c + uVar13 * 9;
      }
      FUN_0047bf50(param_3,(int)*(short *)((int)&DAT_007999dc + iVar11 + 2),
                   (int)*(short *)(&DAT_007999e0 + iVar11),1,1,iVar9,8);
      return;
    }
    FUN_0047c2b0(param_3,(int)*(short *)((int)&DAT_007999dc + iVar11 + 2),
                 (int)*(short *)(&DAT_007999e0 + iVar11),(&DAT_007999db)[iVar11],
                 (&DAT_007999db)[iVar11]);
    return;
  case 100: ;
  case 0x65: ;
  case 0x66: ;
  case 0x67: ;
  case 0x68: ;
  case 0x69: ;
  case 0xc2: ;
  case 0xc3: ;
  case 0xc4: ;
  case 0xe0: ;
    FUN_00488e60(param_3,0);
    return;
  case 0x6a: ;
    iVar11 = DAT_00f569f4;
    break;
  case 0x6b: ;
    iVar11 = FUN_00451bc0(0x19);
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,(&DAT_00f55bb4)[iVar11 * 2],8);
    return;
  case 0x6c: ;
    iVar10 = DAT_00f56624;
    goto LAB_00472378;
  case 0x6d: ;
    iVar10 = DAT_00f5655c;
    goto LAB_00472378;
  case 0x6e: ;
    iVar10 = DAT_00f567bc;
    goto LAB_00472378;
  case 0x6f: ;
    iVar10 = DAT_00f567ec;
    goto LAB_00472378;
  case 0x70: ;
    iVar10 = DAT_00f567f4;
    goto LAB_00472378;
  case 0x71: ;
    iVar10 = DAT_00f567d4;
    goto LAB_00472378;
  case 0x72: ;
    iVar10 = DAT_00f56804;
    goto LAB_00472378;
  case 0x73: ;
    iVar10 = DAT_00f5699c;
    goto LAB_00472378;
  case 0x88: ;
    if (param_7 != 0) {
      uVar7 = FUN_0046efd0(param_3);
      *(undefined1 *)((int)&DAT_00799aa0 + param_3 * 0x108 + 2) = uVar7;
      FUN_004f6520();
      if (DAT_00bca310 == 0) {
        (&DAT_00799a9c)[param_3 * 0x42] = 0;
      }
    }
    FUN_0047cc70(param_4,param_5,0,0);
    *(undefined1 *)(&DAT_00799a82 + param_3 * 0x84) = DAT_00bc96d8;
    cVar8 = FUN_0047a1c0(param_4);
    iVar11 = DAT_00f56bf4;
    if (cVar8 != '\0') {
      iVar11 = DAT_00f57274;
    }
    FUN_0047cf00(param_3,param_4,param_5,iVar11 + DAT_00bc96dc);
    if (DAT_00bca310 == 1) {
      FUN_00485100(DAT_00bc9800);
      FUN_00485100(param_3);
    }
    if (DAT_00bca310 != 2) {
      return;
    }
    FUN_00485100(param_3);
    return;
  case 0x8a: ;
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,DAT_00f5672c,
                 (&DAT_00af598c)[DAT_00bca2d8 + param_5 * 0xe4 + param_4] & 0x100 | 0x48);
    return;
  case 0x8c: ;
  case 0x8d: ;
  case 0x8e: ;
  case 0x8f: ;
  case 0x90: ;
  case 0x91: ;
  case 0x92: ;
  case 0x93: ;
  case 0x94: ;
  case 0x95: ;
    iVar11 = (&DAT_00f55bb4)[*(int *)(&DAT_005d06e4 + param_2 * 0x14) * 2];
    break;
  case 0xa1: ;
    iVar10 = DAT_00f569e4;
    goto LAB_00472378;
  case 0xa2: ;
    iVar10 = DAT_00f569fc;
    goto LAB_00472378;
  case 0xa7: ;
    iVar11 = DAT_00f5668c;
    break;
  case 0xab: ;
  case 0xca: ;
    FUN_004bfb30(param_3);
    return;
  case 0xad: ;
    FUN_00482580(param_4,param_5,2,0,0xff3fbfff);
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,DAT_00f56854,0x8008);
    FUN_0047afd0(param_4,param_5,10);
    return;
  case 0xb1: ;
    iVar10 = DAT_00f566f4;
    goto LAB_00472378;
  case 0xb2: ;
    iVar10 = DAT_00f565e4;
    goto LAB_00472378;
  case 0xb3: ;
    iVar10 = DAT_00f566dc;
    goto LAB_00472378;
  case 0xb4: ;
    iVar11 = DAT_00f56644;
    break;
  case 0xb5: ;
    FUN_0047cc70(param_4,param_5,0,0);
    *(undefined1 *)(&DAT_00799a82 + param_3 * 0x84) = DAT_00bc96d8;
    cVar8 = FUN_0047a1c0(param_4);
    if (cVar8 == '\0') {
      FUN_0047cf00(param_3,param_4,param_5,DAT_00f56bc4 + DAT_00bc96dc);
      return;
    }
    FUN_0047cf00(param_3,param_4,param_5,DAT_00f5725c + DAT_00bc96dc);
    return;
  case 0xb6: ;
    FUN_0047d060(param_4,param_5,0);
    *(undefined1 *)(&DAT_00799a82 + param_3 * 0x84) = DAT_00bc96d8;
    cVar8 = FUN_0047a1c0(param_4);
    iVar11 = DAT_00f56c1c;
    if (cVar8 != '\0') {
      iVar11 = DAT_00f57254;
    }
    FUN_0047d1f0(param_3,param_4,param_5,iVar11 + DAT_00bc96dc);
    if (param_7 == 0) {
      return;
    }
    DAT_00bcb9b0 = DAT_00bcb9b0 + 1;
    return;
  case 0xb7: ;
    if (param_7 != 0) {
      FUN_004ee780(param_1,param_4,param_5,param_6,param_3);
    }
    if (*(short *)(&DAT_00799a10 + param_3 * 0x108) == 0) {
      FUN_004ee4a0(param_3);
    }
    if (param_7 == 0) {
      return;
    }
    FUN_00405ba0();
    return;
  case 0xb8: ;
    iVar11 = DAT_00f5660c;
    break;
  case 0xbc: ;
    (&DAT_00ea5cf6)[DAT_00eaac28 * 0x24ec] = 1;
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,uVar3,8);
    iVar11 = DAT_00eaac28 * 0x49d8;
    if (*(int *)(&DAT_00ea3ffc + iVar11) != 0) {
      return;
    }
    *(int *)(&DAT_00ea4000 + iVar11) = param_3;
    *(undefined2 *)(&DAT_00ea3ff8 + iVar11) = uVar14;
    *(undefined2 *)(&DAT_00ea3ffa + iVar11) = uVar15;
    *(int *)(&DAT_00ea3ffc + iVar11) = DAT_00bca2d8 + param_5 * 0xe4 + param_4;
    return;
  case 0xbd: ;
    (&DAT_00ea5cf6)[DAT_00eaac28 * 0x24ec] = 1;
    uVar18 = uVar19;
LAB_00472125: ;
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,uVar18,8);
    iVar11 = DAT_00eaac28 * 0x49d8;
    if (*(int *)(&DAT_00ea3ffc + iVar11) == 0) {
      *(int *)(&DAT_00ea4000 + iVar11) = param_3;
      *(undefined2 *)(&DAT_00ea3ff8 + iVar11) = uVar14;
      *(undefined2 *)(&DAT_00ea3ffa + iVar11) = uVar15;
      *(int *)(&DAT_00ea3ffc + iVar11) = DAT_00bca2d8 + param_5 * 0xe4 + param_4;
      return;
    }
switchD_004709e5_caseD_26: ;
    return;
  case 0xbe: ;
    iVar11 = DAT_00f564cc;
    if (param_7 != 0) {
      FUN_005596d0();
      uVar7 = FUN_0046b100(param_3);
      *(undefined1 *)((int)&DAT_00799a98 + param_3 * 0x108 + 1) = uVar7;
      iVar11 = DAT_00f564cc;
    }
    break;
  case 199: ;
    iVar11 = DAT_00f56684;
    break;
  case 0xcb: ;
    iVar10 = DAT_00f5657c;
    goto LAB_00472378;
  case 0xcc: ;
    iVar10 = DAT_00f567fc;
    goto LAB_00472378;
  case 0xcd: ;
    iVar10 = DAT_00f567dc;
    goto LAB_00472378;
  case 0xce: ;
    iVar11 = DAT_00f5664c;
    break;
  case 0xcf: ;
  case 0xd0: ;
    if (param_7 != 0) {
      FUN_004e57f0(param_3);
    }
    FUN_004e5840(param_3,param_4,param_5);
    return;
  case 0xd1: ;
    FUN_00440060((&DAT_00799aa8)[param_3 * 0x42],0xd1,param_3);
    *(undefined2 *)((int)&DAT_00ea6082 + DAT_00eaac28 * 0x49d8 + 2) = 1;
    return;
  case 0xd2: ;
    if (param_7 != 0) {
      *(undefined2 *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2) = 1;
      FUN_0041ff90();
      if ((DAT_00bca2e0 == 0) || (DAT_00bca2e0 == 4)) {
        (&DAT_007999ea)[param_3 * 0x84] = 0;
      }
      else {
        (&DAT_007999ea)[param_3 * 0x84] = 3;
      }
      *(undefined2 *)(&DAT_00799a10 + param_3 * 0x108) = 0;
      iVar11 = FUN_0046a400(param_1,0xd2,param_4 + 6,param_5,0,0);
      FUN_00485100(iVar11);
      if ((DAT_00bca2e0 == 0) || (DAT_00bca2e0 == 4)) {
        (&DAT_007999ea)[iVar11 * 0x84] = 1;
      }
      else {
        (&DAT_007999ea)[iVar11 * 0x84] = 4;
      }
      *(undefined2 *)(&DAT_00799a10 + iVar11 * 0x108) = (undefined2)param_8;
      *(short *)(&DAT_00799a12 + param_3 * 0x108) = (short)iVar11;
      *(undefined2 *)(&DAT_00799a12 + iVar11 * 0x108) = 0;
      iVar9 = FUN_0046a400(param_1,0xd2,param_4 + 0xc,param_5,0,0);
      FUN_00485100(iVar9);
      if ((DAT_00bca2e0 == 0) || (DAT_00bca2e0 == 4)) {
        (&DAT_007999ea)[iVar9 * 0x84] = 2;
      }
      else {
        (&DAT_007999ea)[iVar9 * 0x84] = 5;
      }
      *(short *)(&DAT_00799a10 + iVar9 * 0x108) = (short)iVar11;
      *(short *)(&DAT_00799a12 + iVar11 * 0x108) = (short)iVar9;
      *(undefined2 *)(&DAT_00799a12 + iVar9 * 0x108) = 0;
      FUN_004f4f80(param_3);
    }
    iVar11 = FUN_00475b50(param_3);
    if (iVar11 == param_3) {
      if (DAT_00bca2e0 == 0) {
        iVar11 = DAT_00f56d54 + 1;
      }
      else if (DAT_00bca2e0 == 4) {
        iVar11 = DAT_00f56d64 + 1;
      }
      else {
        iVar11 = DAT_00f56d64;
        if (DAT_00bca2e0 != 2) {
          iVar11 = DAT_00f56d54;
        }
      }
      FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,iVar11,8);
      sVar1 = *(short *)(&DAT_00799a12 + param_3 * 0x108);
      if (DAT_00bca2e0 == 0) {
        iVar11 = DAT_00f56d5c + 1;
      }
      else if (DAT_00bca2e0 == 4) {
        iVar11 = DAT_00f56d5c + 1;
      }
      else if ((DAT_00bca2e0 == 2) || (DAT_00bca2e0 == 6)) {
        iVar11 = DAT_00f56d5c;
      }
      FUN_0047bf50((int)sVar1,param_4 + 6,param_5,param_6,param_6,iVar11,8);
      if (DAT_00bca2e0 == 0) {
        iVar9 = DAT_00f56d64 + 1;
      }
      else if (DAT_00bca2e0 == 4) {
        iVar9 = DAT_00f56d54 + 1;
      }
      else {
        iVar9 = DAT_00f56d54;
        if ((DAT_00bca2e0 != 2) && (iVar9 = iVar11, DAT_00bca2e0 == 6)) {
          iVar9 = DAT_00f56d64;
        }
      }
      FUN_0047bf50((int)*(short *)(&DAT_00799a12 + sVar1 * 0x108),param_4 + 0xc,param_5,param_6,
                   param_6,iVar9,8);
    }
    FUN_0053d850();
    return;
  case 0xd3: ;
  case 0xd4: ;
    if (param_7 != 0) {
      *(char *)((int)&DAT_00799a80 + (&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276] * 0x108 + 1) =
           *(char *)((int)&DAT_00799a80 + (&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276] * 0x108 + 1) +
           (param_2 != 0xd3) + '\x01';
    }
    FUN_004c0d50((&DAT_00ea5f9c)[iVar9 * 0x1276],0);
    return;
  case 0xd5: ;
    if (param_7 != 0) {
      FUN_005417b0(param_4,param_5);
      FUN_0041ffc0();
      DAT_006059d4 = 0;
      DAT_006059d8 = 0;
      DAT_006059dc = 0;
      DAT_006059e0 = 0;
      DAT_006059e4 = 0;
      DAT_006059e8 = 0;
      return;
    }
    FUN_00541920(param_3);
    return;
  case 0xd7: ;
    uVar19 = 0x48;
    iVar12 = DAT_00f56b3c;
LAB_00472422: ;
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,iVar12,uVar19);
    return;
  case 0xd8: ;
    iVar11 = FUN_00451bc0(0x1a);
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,(&DAT_00f55bb4)[iVar11 * 2],8);
    return;
  case 0xd9: ;
    iVar10 = DAT_00f56a3c;
    goto LAB_00472378;
  case 0xda: ;
    iVar10 = (DAT_00bca2e0 / 2 & 1U) + DAT_00f57374;
    goto LAB_00472378;
  case 0xdb: ;
    iVar11 = (DAT_00bca2e0 / 2 & 1U) + DAT_00f5737c;
    break;
  case 0xdc: ;
    FUN_0047cf00(param_3,param_4,param_5,DAT_00bca2e0 / 2 + DAT_00f57384);
    return;
  case 0xdd: ;
    iVar11 = DAT_00f56a44;
    break;
  case 0xde: ;
    if (param_7 != 0) {
      FUN_004df9d0(param_4,param_5,param_3,uVar6);
      DAT_006059d4 = 0;
      DAT_006059d8 = 0;
      DAT_006059dc = 0;
      DAT_006059e0 = 0;
      DAT_006059e4 = 0;
      DAT_006059e8 = 0;
      return;
    }
    FUN_004dfbb0(param_3);
    return;
  case 0xe1: ;
    if (param_7 != 0) {
      FUN_0040da90(param_4,param_5,param_3,uVar6);
      FUN_004206a0();
      DAT_006059d4 = 0;
      DAT_006059d8 = 0;
      DAT_006059dc = 0;
      DAT_006059e0 = 0;
      DAT_006059e4 = 0;
      DAT_006059e8 = 0;
      return;
    }
    FUN_0040dca0(param_3);
    return;
  case 0xe2: ;
    iVar11 = DAT_00f57214;
    break;
  case 0xe3: ;
    if (param_7 == 0) {
      FUN_004017a0(param_3);
      return;
    }
    FUN_00417180(param_4,param_5,param_3,0);
    FUN_00420d10();
    FUN_00405ba0();
    return;
  case 0xe4: ;
    if (param_7 != 0) {
      FUN_004e6840(param_4,param_5,param_3,uVar6);
      FUN_00421010();
      DAT_006059d4 = 0;
      DAT_006059d8 = 0;
      DAT_006059dc = 0;
      DAT_006059e0 = 0;
      DAT_006059e4 = 0;
      DAT_006059e8 = 0;
      return;
    }
    FUN_004e69c0(param_3);
    return;
  case 0xe5: ;
    if (param_7 == 0) {
      FUN_005468d0(param_3);
      return;
    }
    FUN_00545c30(param_4,param_5,param_3,uVar6);
    FUN_004212f0();
    DAT_006059e8 = 0;
    return;
  case 0xe6: ;
    if (param_7 != 0) {
      FUN_004015a0(param_4,param_5,param_3,uVar6);
      FUN_004209e0();
      DAT_006059e8 = 0;
      DAT_006059e4 = 0;
      DAT_006059dc = 0;
      DAT_006059e0 = 0;
      DAT_006059d4 = 0;
      DAT_006059d8 = 0;
      return;
    }
    FUN_004017a0(param_3);
    return;
  case 0xe7: ;
    iVar10 = DAT_00f572dc;
    goto LAB_00472378;
  case 0xe8: ;
    iVar10 = DAT_00f572b4;
    goto LAB_00472378;
  case 0xe9: ;
    iVar10 = DAT_00f572bc;
LAB_00472378: ;
    FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,iVar10,8);
    return;
  case 0xea: ;
    if (param_7 == 0) {
      FUN_005468d0(param_3);
      return;
    }
    FUN_00559130(param_4,param_5,param_3,uVar6);
    FUN_004212f0();
    DAT_006059e8 = 0;
    return;
  case 0xeb: ;
    if (param_7 == 0) {
      FUN_005468d0(param_3);
      return;
    }
    FUN_00556480(param_4,param_5,param_3,uVar6);
    FUN_004212f0();
    DAT_006059e8 = 0;
    return;
  case 0xec: ;
    if (param_7 == 0) {
      FUN_005468d0(param_3);
      return;
    }
    FUN_0054fba0(param_4,param_5,param_3,0);
    FUN_004212f0();
    FUN_00405ba0();
    return;
  }
  FUN_0047bf50(param_3,param_4,param_5,param_6,param_6,iVar11,8);
  return;
}
