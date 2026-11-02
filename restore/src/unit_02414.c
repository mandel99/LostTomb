#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005d2a30 (*(undefined *)0x005d2a30u)
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_0077591c (*(undefined *)0x0077591cu)
#define DAT_00775920 (*(undefined *)0x00775920u)
#define DAT_00775924 (*(undefined *)0x00775924u)
#define DAT_00775928 (*(undefined *)0x00775928u)
#define DAT_00776fb0 (*(undefined4 *)0x00776fb0u)
#define DAT_007770e4 (*(undefined *)0x007770e4u)
#define DAT_007770ee (*(undefined4 *)0x007770eeu)
#define DAT_007770f0 (*(undefined4 *)0x007770f0u)
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_0077f75b (*(undefined4 *)0x0077f75bu)
#define DAT_0077f760 (*(undefined4 *)0x0077f760u)
#define DAT_0077f761 (*(undefined4 *)0x0077f761u)
#define DAT_0077f762 (*(undefined4 *)0x0077f762u)
#define DAT_0077f764 (*(undefined4 *)0x0077f764u)
#define DAT_0077f766 (*(undefined4 *)0x0077f766u)
#define DAT_0077f76d (*(undefined4 *)0x0077f76du)
#define DAT_0077f77c (*(undefined4 *)0x0077f77cu)
#define DAT_0077f784 (*(undefined4 *)0x0077f784u)
#define DAT_0077f785 (*(undefined4 *)0x0077f785u)
#define DAT_00d3b550 (*(undefined *)0x00d3b550u)
#define DAT_00d3b554 (*(undefined *)0x00d3b554u)
#define DAT_00d3b558 (*(undefined *)0x00d3b558u)
#define DAT_00d3b55c (*(undefined *)0x00d3b55cu)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00e39088 (*(char *)0x00e39088u)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
#define DAT_010d6a1a (*(undefined4 *)0x010d6a1au)
#define DAT_010d6a1c (*(undefined4 *)0x010d6a1cu)
#define DAT_010d6a32 (*(undefined4 *)0x010d6a32u)
#define DAT_010d6a34 (*(undefined4 *)0x010d6a34u)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern undefined4 FUN_0047d350();
extern void FUN_004cce90();
extern void FUN_004cd890();
extern void FUN_004cd930();
extern void FUN_004cd9d0();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d6860();
extern uint FUN_0052f790();
extern void FUN_0052f850();
extern int FUN_0053e590();

void FUN_0052ec50(void)

{
  char cVar1;
  byte bVar2;
  short sVar3;
  short sVar4;
  undefined1 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  byte *pbVar11;
  bool bVar12;
  undefined4 uVar13;
  
  iVar6 = DAT_00e38dac;
  iVar10 = DAT_00775924;
  if (DAT_00775928 == 1) {
    DAT_00d3b55c = 0x126;
    DAT_00d3b554 = 0x17e;
    DAT_00d3b558 = DAT_00e38dac + -0x67;
    DAT_00d3b550 = DAT_00e38dac + -0xf;
    if (DAT_00775920 == 0) {
      FUN_004cd9d0(0x125,DAT_00e38dac + -0x68,0x5a,0x5a,0xffff);
      FUN_0053e590(iVar10,0x126,DAT_00e38dac + -0x67);
      iVar6 = iVar10;
      if (DAT_0126517c <= iVar10) {
        iVar6 = 0;
      }
      if ((&DAT_010d6a32)[iVar6 * 0x40] != '\0') {
        iVar6 = iVar10;
        if (DAT_0126517c <= iVar10) {
          iVar6 = 0;
        }
        iVar7 = iVar10;
        if (DAT_0126517c <= iVar10) {
          iVar7 = 0;
        }
        iVar8 = iVar10;
        if (DAT_0126517c <= iVar10) {
          iVar8 = 0;
        }
        FUN_0053e590((uint)(byte)(&DAT_010d6a34)[iVar8 * 0x40] + iVar10,
                     (short)(&DAT_010d6a1a)[iVar6 * 0x20] + 0x126,
                     DAT_00e38dac + -0x67 + (int)(short)(&DAT_010d6a1c)[iVar7 * 0x20]);
      }
    }
    DAT_00d3b558 = DAT_00e38db0;
    DAT_00d3b55c = 0;
    DAT_00d3b554 = 0;
    DAT_00d3b550 = DAT_00e38dac;
    FUN_004cd9d0(0x183,DAT_00e38dac + -0x6e,DAT_00e38db0 + -0x198,0x62,0xc618);
    FUN_004cce90(DAT_00775924 + 1,0x40,&DAT_0057c048,0xe1,DAT_00e38dac + -0x1c,0x30,DAT_00f56f8c,0);
    bVar12 = DAT_00775920 != 0;
    FUN_004cd930(0x189,DAT_00e38dac + -0x6a,0xb4,0x14);
    FUN_004d6250(0x2c,8,0x189,DAT_00e38dac + -0x66,0xb4,DAT_00f56f8c,
                 (-(uint)bVar12 & 0xffff0800) + 0xf800,DAT_00e38da8,DAT_00e38da4);
    bVar12 = DAT_00775920 != 1;
    FUN_004cd930(0x189,DAT_00e38dac + -0x51,0xb4,0x14);
    FUN_004d6250(0x2c,9,0x189,DAT_00e38dac + -0x4d,0xb4,DAT_00f56f8c,
                 (-(uint)bVar12 & 0xffff0800) + 0xf800,DAT_00e38da8,DAT_00e38da4);
    bVar12 = DAT_00775920 != 2;
    FUN_004cd930(0x189,DAT_00e38dac + -0x38,0xb4,0x14);
    FUN_004d6250(0x2c,10,0x189,DAT_00e38dac + -0x34,0xb4,DAT_00f56f8c,bVar12 - 1 & 0xf800,
                 DAT_00e38da8,DAT_00e38da4);
    return;
  }
  if (DAT_00775928 == 2) {
    DAT_00d3b55c = 0x126;
    DAT_00d3b554 = 0x17e;
    DAT_00d3b558 = DAT_00e38dac + -0x67;
    DAT_00d3b550 = DAT_00e38dac + -0xf;
    FUN_004cd9d0(0xe1,DAT_00e38dac + -0x36,0x30,0x14,0xf73b);
    FUN_004cce90(DAT_00775924 + 1,0x40,&DAT_0057c048,0xe1,DAT_00e38dac + -0x34,0x30,DAT_00f56f8c,0);
    if ((&DAT_0077f748)[DAT_0077591c * 0x62] == '\0') {
      FUN_004cd9d0(0x125,DAT_00e38dac + -0x68,0x5a,0x5a,0xffff);
      FUN_0053e590(iVar10,0x126,DAT_00e38dac + -0x67);
      iVar6 = iVar10;
      if (DAT_0126517c <= iVar10) {
        iVar6 = 0;
      }
      if ((&DAT_010d6a32)[iVar6 * 0x40] != '\0') {
        iVar6 = iVar10;
        if (DAT_0126517c <= iVar10) {
          iVar6 = 0;
        }
        sVar3 = (&DAT_010d6a1a)[iVar6 * 0x20];
        iVar6 = iVar10;
        if (DAT_0126517c <= iVar10) {
          iVar6 = 0;
        }
        sVar4 = (&DAT_010d6a1c)[iVar6 * 0x20];
        iVar6 = iVar10;
        if (DAT_0126517c <= iVar10) {
          iVar6 = 0;
        }
        uVar5 = FUN_0052f790(iVar10,(&DAT_010d6a34)[iVar6 * 0x40]);
        (&DAT_010d6a34)[iVar6 * 0x40] = uVar5;
        iVar6 = iVar10;
        if (DAT_0126517c <= iVar10) {
          iVar6 = 0;
        }
        FUN_0053e590((uint)(byte)(&DAT_010d6a34)[iVar6 * 0x40] + iVar10,sVar3 + 0x126,
                     DAT_00e38dac + -0x67 + (int)sVar4);
      }
    }
    DAT_00d3b558 = DAT_00e38db0;
    DAT_00d3b550 = DAT_00e38dac;
    DAT_00d3b55c = 0;
    DAT_00d3b554 = 0;
    FUN_004cd9d0(0x183,DAT_00e38dac + -0x6e,DAT_00e38db0 + -0x198,0x62,0xf73b);
    FUN_004cce90(DAT_00775924 + 1,0x40,&DAT_0057c048,0xe1,DAT_00e38dac + -0x1c,0x30,DAT_00f56f8c,0);
    iVar10 = DAT_0077591c;
    DAT_00e20778 = 0;
    iVar6 = DAT_0077591c * 0x62;
    cVar1 = (&DAT_0077f748)[iVar6];
    if (cVar1 == '\x01') {
      FUN_004cd930(0x189,DAT_00e38dac + -0x70,0xb4,0x14);
      FUN_004d6250(0x27,(&DAT_0077f760)[iVar6],0x189,DAT_00e38dac + -0x6c,0xb4,DAT_00f56f8c,0,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004cd930(0x251,DAT_00e38dac + -0x70,0xb4,0x14);
      if (DAT_00e39088 == '\0') {
        uVar5 = (&DAT_0077f761)[iVar6];
        uVar13 = 0xc3;
      }
      else {
        uVar5 = (&DAT_0077f761)[iVar6];
        uVar13 = 0x15;
      }
      FUN_004d6250(uVar13,uVar5,0x251,DAT_00e38dac + -0x6c,0xb4,DAT_00f56f8c,0,DAT_00e38da8,
                   DAT_00e38da4);
      FUN_004cd930(0x125,DAT_00e38dac + -0x70,100,0x14);
      FUN_004d6250(0x56,(&DAT_0077f75b)[iVar6],0x125,DAT_00e38dac + -0x6c,100,DAT_00f56f8c,0,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004d62f0(0x2c,0x20,0x189,DAT_00e38dac + -0x53,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004cd930(0x1c5,DAT_00e38dac + -0x57,0x32,0x14);
      FUN_004cce90((&DAT_0077f762)[iVar6],0x40,&DAT_0057c048,0x1c5,DAT_00e38dac + -0x53,0x32,
                   DAT_00f56f8c,0);
      if ((&DAT_0077f760)[iVar6] != '\0') {
        FUN_004cd930(0x20b,DAT_00e38dac + -0x57,100,0x14);
        FUN_004d6250(0x2c,0x21,0x20b,DAT_00e38dac + -0x53,100,DAT_00f56f8c,0,DAT_00e38da8,
                     DAT_00e38da4);
      }
      FUN_004d62f0(0x2c,0x22,0x28d,DAT_00e38dac + -0x53,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004cd930(0x2bf,DAT_00e38dac + -0x57,0x50,0x14);
      FUN_004cce90((int)(short)(&DAT_0077f764)[iVar10 * 0x31],0x40,&DAT_0057c048,0x2bf,
                   DAT_00e38dac + -0x53,0x3c,DAT_00f56f8c,0);
      if ((&DAT_0077f760)[iVar6] == '\0') {
        pbVar11 = &DAT_0077f76d + iVar6;
        iVar10 = 399;
        do {
          FUN_004cd890(iVar10 + -1,DAT_00e38dac + -0x3e,0x1a,0x1a);
          iVar8 = DAT_00e38dac + -0x3d;
          bVar2 = pbVar11[-7];
          iVar6 = DAT_00f571fc;
          iVar7 = FUN_0047d350((uint)bVar2,3);
          FUN_0053e590(iVar7 + iVar6 + (uint)bVar2,iVar10,iVar8);
          FUN_004cd890(iVar10 + -1,DAT_00e38dac + -0x22,0x1a,0x1a);
          iVar6 = DAT_00f571fc;
          iVar7 = DAT_00e38dac + -0x21;
          bVar2 = *pbVar11;
          iVar8 = FUN_0047d350((uint)bVar2,3);
          FUN_0053e590(iVar8 + iVar6 + (uint)bVar2,iVar10,iVar7);
          iVar10 = iVar10 + 0x1b;
          pbVar11 = pbVar11 + 1;
        } while (iVar10 < 0x24c);
        return;
      }
      FUN_004d62f0(0x2c,0x24,0x189,DAT_00e38dac + -0x37,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      pbVar11 = &DAT_0077f766 + iVar6;
      iVar10 = 0x20c;
      do {
        FUN_004cd890(iVar10 + -1,DAT_00e38dac + -0x3e,0x1a,0x1a);
        iVar9 = DAT_00e38dac + -0x3d;
        bVar2 = *pbVar11;
        iVar7 = DAT_00f571fc;
        iVar8 = FUN_0047d350((uint)bVar2,3);
        FUN_0053e590(iVar8 + iVar7 + (uint)bVar2,iVar10,iVar9);
        iVar10 = iVar10 + 0x1e;
        pbVar11 = pbVar11 + 1;
      } while (iVar10 < 0x2fc);
      FUN_004d62f0(0x2c,5,0x189,DAT_00e38dac + -0x1b,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      pbVar11 = &DAT_0077f77c + iVar6;
      iVar10 = 0x20c;
      do {
        FUN_004cd890(iVar10 + -1,DAT_00e38dac + -0x22,0x1a,0x1a);
        iVar6 = DAT_00f571fc;
        iVar7 = DAT_00e38dac + -0x21;
        bVar2 = *pbVar11;
        iVar8 = FUN_0047d350((uint)bVar2,3);
        FUN_0053e590(iVar8 + iVar6 + (uint)bVar2,iVar10,iVar7);
        iVar10 = iVar10 + 0x1e;
        pbVar11 = pbVar11 + 1;
      } while (iVar10 < 0x2fc);
      return;
    }
    if (cVar1 == '\x03') {
      FUN_004d62f0(0x2c,0x45,0x189,DAT_00e38dac + -100,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004d62f0(0x2c,0x46,0x1a7,DAT_00e38dac + -0x40,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004cd930(0x20b,DAT_00e38dac + -0x44,0x32,0x14);
      FUN_004cce90((&DAT_0077f784)[iVar6],0x40,&DAT_0057c048,0x20b,DAT_00e38dac + -0x40,0x32,
                   DAT_00f56f8c,0);
      FUN_004d62f0(0x2c,0x47,0x1a7,DAT_00e38dac + -0x22,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004cd930(0x20b,DAT_00e38dac + -0x26,0x32,0x14);
      FUN_004cce90((&DAT_0077f785)[iVar6],0x40,&DAT_0057c048,0x20b,DAT_00e38dac + -0x22,0x32,
                   DAT_00f56f8c,0);
      return;
    }
    if (cVar1 == '\x02') {
      FUN_004cd930(0x189,DAT_00e38dac + -0x54,0xb4,0x14);
      FUN_004d6250(0xc4,(&DAT_0077f761)[iVar6],0x189,DAT_00e38dac + -0x54,0xb4,DAT_00f56f84,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    return;
  }
  if (DAT_00775928 != 3) {
    if (DAT_00775928 == 4) {
      FUN_0052f850();
      return;
    }
    if (DAT_00775928 == 9) {
      FUN_0052f850();
      return;
    }
    if (DAT_00775928 == 10) {
      FUN_0052f850();
      return;
    }
    if (DAT_00775928 == 0xb) {
      FUN_0052f850();
      return;
    }
    if (DAT_00775928 == 0xc) {
      FUN_0052f850();
      return;
    }
    if (DAT_00775928 == 0xd) {
      FUN_0052f850();
      return;
    }
    if ((DAT_00775928 == 0xe) || (DAT_00775928 == 0xf)) {
      FUN_004cd930(0x189,DAT_00e38dac + -0x68,0xb4,0x14);
      iVar10 = (-(uint)(DAT_007056c8 != 1) & 0xffff0800) + 0xf800;
      DAT_00e20778 = 0;
      FUN_004d6160(0x2c,0x20,0x19d,iVar6 + -100,100,DAT_00f56f8c,iVar10,DAT_00e38da8,DAT_00e38da4);
      FUN_004d6860(DAT_005d2a30,0x40,&DAT_007027d4,DAT_00e20778 + 0x19d,iVar6 + -100,DAT_00f56f8c,
                   iVar10,DAT_00e38da8,DAT_00e38da4);
      FUN_004cd930(0x189,iVar6 + -0x4f,0xb4,0x14);
      if ((&DAT_007770f0)[DAT_005d2a30 * 0x144] == '\x01') {
        uVar13 = 0xab;
      }
      else if ((&DAT_007770f0)[DAT_005d2a30 * 0x144] == '\x02') {
        uVar13 = 0xaa;
      }
      else {
        uVar13 = 0xa9;
      }
      FUN_004d6160(0x2c,uVar13,0x19d,iVar6 + -0x4b,0xb4,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004cd930(0x189,iVar6 + -0x1d,0xb4,0x14);
      DAT_00e20778 = 0;
      FUN_004d6160(0x2c,0xa8,0x19d,iVar6 + -0x19,0xb4,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      iVar10 = (int)*(short *)(DAT_005d2a30 * 0x144 + 0x7770ec);
      if (iVar10 != -1) {
        if (DAT_00e39088 == '\0') {
          uVar5 = (&DAT_0077f761)[iVar10 * 0x62];
          uVar13 = 0xc3;
        }
        else {
          uVar5 = (&DAT_0077f761)[iVar10 * 0x62];
          uVar13 = 0x15;
        }
        FUN_004d6250(uVar13,uVar5,0x251,DAT_00e38dac + -0x6c,0xb4,DAT_00f56f8c,0,DAT_00e38da8,
                     DAT_00e38da4);
      }
      iVar10 = (int)*(short *)(&DAT_007770ee + DAT_005d2a30 * 0x144);
      if (iVar10 != -1) {
        if (DAT_00e39088 == '\0') {
          uVar5 = (&DAT_0077f761)[iVar10 * 0x62];
          uVar13 = 0xc3;
        }
        else {
          uVar5 = (&DAT_0077f761)[iVar10 * 0x62];
          uVar13 = 0x15;
        }
        FUN_004d6250(uVar13,uVar5,0x251,DAT_00e38dac + -0x58,0xb4,DAT_00f56f8c,0,DAT_00e38da8,
                     DAT_00e38da4);
      }
      FUN_004cd9d0(0x251,DAT_00e38dac + -0x44,0xb4,0x14,0xf73b);
      DAT_00e20778 = 0;
      FUN_004d6160(0x2c,0xad,0x251,DAT_00e38dac + -0x44,0xb4,DAT_00f56f8c,0,DAT_00e38da8,
                   DAT_00e38da4);
      FUN_004d6860((&DAT_007770e4)[DAT_005d2a30 * 0x51] *
                   (uint)(byte)(&DAT_00776fb0)[DAT_005d2a30 * 0x144],0x40,&DAT_007027d4,
                   DAT_00e20778 + 0x251,DAT_00e38dac + -0x44,DAT_00f56f8c,0,DAT_00e38da8,
                   DAT_00e38da4);
      return;
    }
  }
  return;
}
