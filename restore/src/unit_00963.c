#include "restore_types.h"
#define DAT_005d06dc (*(undefined4 *)0x005d06dcu)
#define DAT_005d06e0 (*(undefined4 *)0x005d06e0u)
#define DAT_0070ce0c (*(undefined4 *)0x0070ce0cu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d7b4 (*(undefined *)0x0078d7b4u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999d9 (*(undefined4 *)0x007999d9u)
#define DAT_007999da (*(undefined4 *)0x007999dau)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999ee (*(undefined4 *)0x007999eeu)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_00799a0c (*(undefined4 *)0x00799a0cu)
#define DAT_00799a1b (*(undefined4 *)0x00799a1bu)
#define DAT_00799a24 (*(undefined4 *)0x00799a24u)
#define DAT_00799a25 (*(undefined4 *)0x00799a25u)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a76 (*(undefined4 *)0x00799a76u)
#define DAT_00799a78 (*(undefined4 *)0x00799a78u)
#define DAT_00799a7a (*(undefined4 *)0x00799a7au)
#define DAT_00799a7e (*(undefined4 *)0x00799a7eu)
#define DAT_00799a80 (*(undefined4 *)0x00799a80u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00799a9a (*(undefined4 *)0x00799a9au)
#define DAT_00799a9c (*(undefined4 *)0x00799a9cu)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00799abc (*(undefined4 *)0x00799abcu)
#define DAT_00799ac0 (*(undefined4 *)0x00799ac0u)
#define DAT_00799ad4 (*(undefined4 *)0x00799ad4u)
#define DAT_00799ae0 (*(undefined4 *)0x00799ae0u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00bc9714 (*(undefined *)0x00bc9714u)
#define DAT_00bc9800 (*(undefined *)0x00bc9800u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca310 (*(undefined *)0x00bca310u)
#define DAT_00ea1874 (*(undefined4 *)0x00ea1874u)
extern void FUN_0046a280();
extern undefined4 FUN_00481000();
extern undefined4 FUN_004851b0();
extern undefined4 FUN_00488050();
extern void FUN_004e2970();
extern void FUN_004e7f00();

int FUN_0046a400(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  byte bVar1;
  short sVar2;
  char *pcVar3;
  undefined1 uVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  char *pcVar9;
  uint uVar10;
  
  if (param_6 == 0) {
    param_6 = 1;
    pcVar9 = &DAT_00799ae0;
    iVar6 = param_5;
    do {
      iVar6 = iVar6 + 1;
      if ((*pcVar9 == '\0') && (iVar8 = FUN_004851b0(param_6), iVar8 == 0)) {
        if ((param_5 < 2) ||
           (iVar8 = iVar6 + (1 - param_5), iVar7 = iVar8, pcVar3 = pcVar9, iVar6 <= iVar8))
        goto LAB_0046a43b;
        do {
          if (pcVar3[0x108] != '\0') goto LAB_0046a556;
          iVar7 = iVar7 + 1;
          pcVar3 = pcVar3 + 0x108;
        } while (iVar7 < iVar6);
        while( true ) {
          if (iVar6 <= iVar8) goto LAB_0046a43b;
          iVar7 = FUN_004851b0(iVar8);
          if (iVar7 != 0) break;
          iVar8 = iVar8 + 1;
        }
      }
LAB_0046a556: ;
      pcVar9 = pcVar9 + 0x108;
      param_6 = param_6 + 1;
    } while ((int)pcVar9 < 0x89b6d8);
  }
  else if (((&DAT_007999d8)[param_6 * 0x108] == '\0') && (iVar6 = FUN_004851b0(param_6), iVar6 == 0)
          ) {
LAB_0046a43b: ;
    iVar6 = param_6 * 0x108;
    (&DAT_007999d8)[iVar6] = 3;
    (&DAT_007999d9)[iVar6] = (char)param_1;
    (&DAT_007999da)[iVar6] = *(undefined1 *)((int)&DAT_00ea1874 + param_1 * 0x49d8 + 1);
    uVar4 = (&DAT_005d06dc)[param_2 * 0x14];
    (&DAT_007999e8)[param_6 * 0x84] = (short)param_2;
    (&DAT_007999db)[iVar6] = uVar4;
    *(short *)(&DAT_007999ee + iVar6) = (short)DAT_0078d7b4;
    DAT_0078d7b4 = DAT_0078d7b4 + 1;
    *(undefined1 *)(&DAT_00799a9a + param_6 * 0x84) = 0x32;
    (&DAT_007999f8)[param_6 * 0x84] = 0;
    *(undefined1 *)((int)&DAT_007999dc + iVar6 + 1) = 0;
    sVar2 = (&DAT_007999e8)[param_6 * 0x84];
    if ((sVar2 < 10) || (0x13 < sVar2)) {
      if ((sVar2 < 0x14) || (0x17 < sVar2)) {
        if ((sVar2 < 0x18) || (0x1b < sVar2)) {
          if ((0x1b < sVar2) && (sVar2 < 0x1e)) {
            *(undefined1 *)((int)&DAT_007999dc + iVar6 + 1) = 4;
          }
        }
        else {
          *(undefined1 *)((int)&DAT_007999dc + iVar6 + 1) = 3;
        }
      }
      else {
        *(undefined1 *)((int)&DAT_007999dc + iVar6 + 1) = 2;
      }
    }
    else {
      *(undefined1 *)((int)&DAT_007999dc + iVar6 + 1) = 1;
    }
    if (((short)(&DAT_007999e8)[param_6 * 0x84] < 10) ||
       (0x1d < (short)(&DAT_007999e8)[param_6 * 0x84])) {
      (&DAT_007999ea)[param_6 * 0x84] = 0;
    }
    else {
      (&DAT_007999ea)[param_6 * 0x84] = (short)param_2 + -10;
    }
    *(undefined2 *)((int)&DAT_00799aa4 + iVar6 + 2) = 0;
    if (*(char *)((int)&DAT_007999dc + iVar6 + 1) == '\0') {
      (&DAT_00799abc)[param_6 * 0x42] = 0;
    }
    else {
      (&DAT_00799abc)[param_6 * 0x42] =
           *(undefined4 *)(&DAT_0070ce0c + (short)(&DAT_007999ea)[param_6 * 0x84] * 0x58);
      (&DAT_00799ac0)[param_6 * 0x42] = 0;
      *(undefined4 *)(&DAT_00799ad4 + iVar6) = 0;
      FUN_004e7f00(param_6,1);
    }
    sVar2 = (&DAT_007999e8)[param_6 * 0x84];
    if (((sVar2 < 0x8c) || (0x95 < sVar2)) && (sVar2 != 0xca)) {
      if ((0x28 < sVar2) && (sVar2 < 0x2c)) {
        uVar10 = DAT_00bc9714 & 0x80000003;
        if ((int)uVar10 < 0) {
          uVar10 = (uVar10 - 1 | 0xfffffffc) + 1;
        }
        iVar8 = (int)(DAT_00bc9714 + 4 + ((int)(DAT_00bc9714 + 4) >> 0x1f & 3U)) >> 2;
        if (3 < iVar8) {
          iVar8 = 0;
        }
        iVar8 = DAT_00bca2e0 / 2 + 1 + iVar8;
        if (3 < iVar8) {
          iVar8 = iVar8 + -4;
        }
        *(char *)(&DAT_00799a82 + param_6 * 0x84) = (char)uVar10 * '\x10' + (char)iVar8;
      }
    }
    else {
      *(undefined1 *)(&DAT_00799a82 + param_6 * 0x84) = (undefined1)DAT_00bc9714;
    }
    sVar2 = (&DAT_007999e8)[param_6 * 0x84];
    if (sVar2 == 0x66) {
      (&DAT_00799a1b)[iVar6] = 1;
    }
    else if (sVar2 == 0x67) {
      (&DAT_00799a1b)[iVar6] = 3;
    }
    else if (sVar2 == 0x68) {
      (&DAT_00799a1b)[iVar6] = 5;
    }
    else if (sVar2 == 0x65) {
      (&DAT_00799a1b)[iVar6] = 0x10;
    }
    else if (sVar2 == 0xe0) {
      (&DAT_00799a1b)[iVar6] = 0x20;
    }
    else if (sVar2 == 0x69) {
      (&DAT_00799a1b)[iVar6] = 4;
    }
    else if (sVar2 == 100) {
      (&DAT_00799a1b)[iVar6] = 0xe;
    }
    else if (sVar2 == 0xc2) {
      (&DAT_00799a1b)[iVar6] = 2;
      (&DAT_007999ea)[param_6 * 0x84] = 0xd;
    }
    else if (sVar2 == 0xc4) {
      (&DAT_00799a1b)[iVar6] = 6;
    }
    else if (sVar2 == 0xc3) {
      (&DAT_00799a1b)[iVar6] = 0x16;
    }
    else if (sVar2 == 0x6a) {
      (&DAT_00799a1b)[iVar6] = 0x18;
    }
    else if (sVar2 == 0xdd) {
      (&DAT_00799a1b)[iVar6] = 0x1e;
    }
    else if (sVar2 == 0x6b) {
      (&DAT_00799a1b)[iVar6] = 0x19;
    }
    else if (sVar2 == 0xd8) {
      (&DAT_00799a1b)[iVar6] = 0x1a;
    }
    else if (sVar2 == 0xa1) {
      (&DAT_00799a1b)[iVar6] = 0x15;
    }
    else if (sVar2 == 0xd9) {
      (&DAT_00799a1b)[iVar6] = 0x1d;
    }
    else if (sVar2 == 0xa2) {
      (&DAT_00799a1b)[iVar6] = 0x12;
    }
    else if (sVar2 == 0x6c) {
      (&DAT_00799a1b)[iVar6] = 0x14;
    }
    else if (sVar2 == 0x6d) {
      (&DAT_00799a1b)[iVar6] = 0xb;
    }
    else if (sVar2 == 0x6e) {
      (&DAT_00799a1b)[iVar6] = 0xf;
      (&DAT_007999ea)[param_6 * 0x84] = 9;
    }
    else if (sVar2 == 0x6f) {
      (&DAT_00799a1b)[iVar6] = 0x11;
      (&DAT_007999ea)[param_6 * 0x84] = 1;
    }
    else if (sVar2 == 0xcb) {
      (&DAT_00799a1b)[iVar6] = 0x17;
      (&DAT_007999ea)[param_6 * 0x84] = 0xb;
    }
    else if (sVar2 == 0xcc) {
      (&DAT_00799a1b)[iVar6] = 0xc;
      (&DAT_007999ea)[param_6 * 0x84] = 5;
      *(undefined2 *)((int)&DAT_00799aa4 + iVar6 + 2) = 0xd;
    }
    else if (sVar2 == 0xe8) {
      (&DAT_00799a1b)[iVar6] = 0x22;
      (&DAT_007999ea)[param_6 * 0x84] = 0x17;
      *(undefined2 *)((int)&DAT_00799aa4 + iVar6 + 2) = 0x18;
    }
    else if (sVar2 == 0xe9) {
      (&DAT_00799a1b)[iVar6] = 0x21;
      (&DAT_007999ea)[param_6 * 0x84] = 0x14;
    }
    else if (sVar2 == 0xcd) {
      (&DAT_00799a1b)[iVar6] = 0x1c;
      (&DAT_007999ea)[param_6 * 0x84] = 4;
    }
    else if (sVar2 == 0x70) {
      (&DAT_00799a1b)[iVar6] = 10;
      (&DAT_007999ea)[param_6 * 0x84] = 0x11;
    }
    else if (sVar2 == 0x71) {
      (&DAT_00799a1b)[iVar6] = 0x13;
      (&DAT_007999ea)[param_6 * 0x84] = 10;
    }
    else if (sVar2 == 0x72) {
      (&DAT_00799a1b)[iVar6] = 0xd;
      (&DAT_007999ea)[param_6 * 0x84] = 5;
    }
    else if (sVar2 == 0xb1) {
      (&DAT_00799a1b)[iVar6] = 0;
      (&DAT_007999ea)[param_6 * 0x84] = 4;
    }
    else if (sVar2 == 0xe7) {
      (&DAT_00799a1b)[iVar6] = 0;
      (&DAT_007999ea)[param_6 * 0x84] = 0x19;
      *(undefined2 *)((int)&DAT_00799aa4 + iVar6 + 2) = 5;
    }
    else if ((sVar2 == 0xb2) || (sVar2 == 0xb3)) {
      (&DAT_00799a1b)[iVar6] = 0;
      (&DAT_007999ea)[param_6 * 0x84] = 0;
    }
    else if (sVar2 == 0x5f) {
      (&DAT_00799a1b)[iVar6] = 0;
      (&DAT_007999ea)[param_6 * 0x84] = 8;
      *(undefined2 *)((int)&DAT_00799aa4 + iVar6 + 2) = 0xe;
    }
    else if (sVar2 == 0x4a) {
      (&DAT_00799a1b)[iVar6] = 0;
      (&DAT_007999ea)[param_6 * 0x84] = 4;
      FUN_0046a280(param_6);
    }
    else if (((sVar2 == 0xbb) || (sVar2 == 0xbc)) || (sVar2 == 0xbd)) {
      (&DAT_00799a1b)[iVar6] = 0;
      (&DAT_007999ea)[param_6 * 0x84] = 0xf;
    }
    else if ((sVar2 == 0x35) || (sVar2 == 0x33)) {
      (&DAT_00799a1b)[iVar6] = 0;
      (&DAT_007999ea)[param_6 * 0x84] = 0xc;
    }
    else if (sVar2 == 0x2f) {
      (&DAT_00799a1b)[iVar6] = 0;
      (&DAT_007999ea)[param_6 * 0x84] = 0x10;
    }
    else if (sVar2 == 0x20) {
      (&DAT_00799a1b)[iVar6] = 0;
      (&DAT_007999ea)[param_6 * 0x84] = 0x13;
    }
    else {
      (&DAT_00799a1b)[iVar6] = 0;
      if (sVar2 == 0xe2) {
        (&DAT_007999ea)[param_6 * 0x84] = 0x16;
        *(undefined2 *)((int)&DAT_00799aa4 + iVar6 + 2) = 0xd;
      }
    }
    if (((&DAT_007999e8)[param_6 * 0x84] == 0x47) || ((&DAT_007999e8)[param_6 * 0x84] == 0xbf)) {
      (&DAT_00799a2e)[param_6 * 0x84] = 0xc80;
    }
    iVar8 = DAT_00bca2d8;
    *(short *)((int)&DAT_007999dc + iVar6 + 2) = (short)param_3;
    *(short *)(&DAT_007999e0 + iVar6) = (short)param_4;
    iVar8 = iVar8 + param_4 * 0xe4 + param_3;
    *(int *)(&DAT_007999e4 + iVar6) = iVar8;
    bVar1 = (&DAT_00acf85c)[iVar8];
    (&DAT_00799a25)[iVar6] = bVar1 & 0x7f;
    (&DAT_00799a0c)[iVar6] = bVar1 & 6;
    (&DAT_00799a24)[iVar6] = (&DAT_005d06e0)[param_2 * 0x14];
    uVar4 = FUN_00481000(param_3,param_4,(&DAT_007999db)[iVar6]);
    *(undefined1 *)(&DAT_00799a98 + param_6 * 0x84) = uVar4;
    iVar8 = DAT_00bc9800;
    if ((&DAT_007999e8)[param_6 * 0x84] == 0x88) {
      if (DAT_00bca310 == 0) {
        DAT_00bc9800 = param_6;
      }
      else if ((DAT_00bca310 == 1) || (DAT_00bca310 == 2)) {
        (&DAT_00799a9c)[param_6 * 0x42] = DAT_00bc9800;
        (&DAT_00799a9c)[iVar8 * 0x42] = param_6;
      }
    }
    sVar2 = (&DAT_007999e8)[param_6 * 0x84];
    if (((sVar2 == 0x1f) || (sVar2 == 0x1e)) || (sVar2 == 0x21)) {
      *(undefined2 *)(&DAT_00799a76 + iVar6) = 0;
      *(undefined2 *)(&DAT_00799a78 + iVar6) = 0;
      *(undefined2 *)(&DAT_00799a7a + param_6 * 0x42) = 0;
      *(undefined2 *)((int)&DAT_00799a7a + iVar6 + 2) = 0;
      *(undefined1 *)((int)&DAT_00799a7e + iVar6 + 1) = 0;
      *(undefined1 *)(&DAT_00799a80 + param_6 * 0x84) = 0;
      *(undefined1 *)((int)&DAT_00799a80 + iVar6 + 1) = 0;
    }
    sVar2 = (&DAT_007999e8)[param_6 * 0x84];
    if (((((sVar2 == 0x73) || (sVar2 == 0x6c)) ||
         ((sVar2 == 0xb1 || ((sVar2 == 0xb2 || (sVar2 == 0xb3)))))) || (sVar2 == 0xe7)) ||
       (((sVar2 == 0xc3 || (cVar5 = FUN_00488050(param_6), cVar5 != '\0')) ||
        ((&DAT_007999e8)[param_6 * 0x84] == 199)))) {
      *(undefined1 *)(&DAT_00799a7e + param_6 * 0x84) = 0;
    }
    if (DAT_0078d790 <= param_6) {
      DAT_0078d790 = param_6;
    }
    return param_6;
  }
  FUN_004e2970(0x41,0,0);
  return 0;
}
