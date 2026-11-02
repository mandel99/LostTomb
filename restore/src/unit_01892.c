#include "restore_types.h"
#define DAT_005d1960 (*(undefined4 *)0x005d1960u)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0077590c (*(undefined *)0x0077590cu)
#define DAT_00775910 (*(undefined *)0x00775910u)
#define DAT_007843d8 (*(undefined4 *)0x007843d8u)
#define DAT_007843da (*(undefined4 *)0x007843dau)
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_007848dc (*(undefined *)0x007848dcu)
#define DAT_007848f8 (*(undefined *)0x007848f8u)
#define DAT_00784c48 (*(undefined *)0x00784c48u)
#define DAT_00784c4c (*(undefined *)0x00784c4cu)
#define DAT_00784c50 (*(undefined *)0x00784c50u)
#define DAT_00784c54 (*(undefined *)0x00784c54u)
#define DAT_00784f10 (*(undefined *)0x00784f10u)
#define DAT_0078d7b8 (*(undefined *)0x0078d7b8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea3fe8 (*(undefined4 *)0x00ea3fe8u)
#define DAT_00ea3fea (*(undefined4 *)0x00ea3feau)
#define DAT_00ea3ff0 (*(undefined4 *)0x00ea3ff0u)
#define DAT_00ea3ff2 (*(undefined4 *)0x00ea3ff2u)
#define DAT_00ea3ff4 (*(undefined4 *)0x00ea3ff4u)
#define DAT_00ea452c (*(undefined4 *)0x00ea452cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00784c64 (*(undefined4 *)0x00784c64u)
#define _DAT_00784c66 (*(undefined4 *)0x00784c66u)
#define _DAT_00784c68 (*(undefined4 *)0x00784c68u)
#define _DAT_00784c6a (*(undefined4 *)0x00784c6au)
#define _DAT_00784c8c (*(undefined4 *)0x00784c8cu)
#define _DAT_00784c8e (*(undefined4 *)0x00784c8eu)
#define _DAT_00784c90 (*(undefined4 *)0x00784c90u)
#define _DAT_00784c92 (*(undefined4 *)0x00784c92u)
extern void FUN_00404350();
extern void FUN_004051a0();
extern void FUN_0040f860();
extern void FUN_00442800();
extern void FUN_00444720();
extern void FUN_00444b10();
extern void FUN_004bbd40();
extern int FUN_004cde40();
extern void FUN_004de600();
extern void FUN_00517370();
extern void FUN_00517660();
extern void FUN_005178b0();
extern void FUN_005178e0();

void FUN_004de820(void)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  ushort uVar4;
  int iVar5;
  ushort uVar6;
  undefined4 uVar7;
  
  uVar7 = 0;
  DAT_00eaac28 = 1;
  FUN_004de600();
  FUN_0040f860(&DAT_007848d8,&DAT_007848d8,0x638);
  if (DAT_00605a3c == 0) {
    if ((((_DAT_00784c8c != -1) && (_DAT_00784c8e != -1)) && (_DAT_00784c90 != -1)) &&
       (_DAT_00784c92 != -1)) {
      uVar7 = 1;
    }
    FUN_004bbd40((int)_DAT_00784c8c,(int)_DAT_00784c8e,uVar7);
  }
  FUN_005178b0();
  FUN_00517370();
  FUN_00517660();
  FUN_005178e0();
  if ((_DAT_00784c64 == 0xffff) || (_DAT_00784c66 == 0xffff)) {
    _DAT_00784c64 = (short)DAT_00bca2d0 - 1;
    _DAT_00784c66 = (ushort)(DAT_00bca2d4 / 2);
  }
  uVar1 = _DAT_00784c66;
  uVar4 = _DAT_00784c64;
  iVar5 = DAT_00eaac28 * 0x49d8;
  *(ushort *)(&DAT_00ea3fe8 + iVar5) = _DAT_00784c64 & 0xff;
  uVar6 = _DAT_00784c66 & 0xff;
  *(ushort *)(&DAT_00ea3fea + iVar5) = uVar6;
  iVar2 = DAT_00bca2d8;
  *(int *)(iVar5 + 0xea3fec) =
       DAT_00bca2d8 + *(short *)(&DAT_00ea3fe8 + iVar5) + (short)uVar6 * 0xe4;
  if ((_DAT_00784c68 == 0xffff) || (_DAT_00784c6a == 0xffff)) {
    _DAT_00784c68 = uVar4;
    _DAT_00784c6a = uVar1;
  }
  *(ushort *)(&DAT_00ea3ff0 + iVar5) = _DAT_00784c68 & 0xff;
  uVar4 = _DAT_00784c6a & 0xff;
  *(ushort *)(&DAT_00ea3ff2 + iVar5) = uVar4;
  iVar3 = DAT_00e39080;
  *(int *)(&DAT_00ea3ff4 + iVar5) = iVar2 + *(short *)(&DAT_00ea3ff0 + iVar5) + (short)uVar4 * 0xe4;
  uVar7 = FUN_004cde40(DAT_007848f8,*(undefined4 *)(&DAT_005d1960 + iVar3 * 4));
  iVar2 = DAT_00eaac28;
  *(undefined4 *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) = uVar7;
  (&DAT_00ea452c)[iVar2 * 0x1276] = uVar7;
  DAT_0078d7bc = (int)DAT_007848d8;
  if (DAT_00784c48 == 0) {
    if (DAT_00784c50 == 0) {
      DAT_0078d7b8 = DAT_0078d7bc + 1000000;
    }
    else {
      DAT_0078d7b8 = DAT_0078d7bc + DAT_00784c54;
    }
  }
  else {
    DAT_0078d7b8 = DAT_0078d7bc + DAT_00784c4c;
  }
  DAT_00784f10 = (int)DAT_007848dc;
  FUN_00444720();
  FUN_00444b10();
  DAT_00775910 = (int)*(short *)(&DAT_007843d8 + DAT_00784f10 * 0x20);
  DAT_0077590c = (int)*(short *)(&DAT_007843da + DAT_00784f10 * 0x20);
  FUN_00442800();
  FUN_00404350();
  FUN_004051a0();
  return;
}
