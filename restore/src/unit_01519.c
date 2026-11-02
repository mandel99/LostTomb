#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00784b74 (*(undefined *)0x00784b74u)
#define DAT_00784cb1 (*(undefined1 *)0x00784cb1u)
#define DAT_00bcd5f9 (*(undefined4 *)0x00bcd5f9u)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd631 (*(undefined4 *)0x00bcd631u)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern void FUN_004b4220();
extern int FUN_004b4800();
extern void FUN_004b4980();
extern uint FUN_00562348();

void FUN_004bbd40(undefined4 param_1,undefined4 param_2,int param_3)

{
  short sVar1;
  byte bVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  char *pcVar8;
  short *psVar9;
  int iVar10;
  int local_4;
  
  if (DAT_00605a3c == 0) {
    psVar9 = &DAT_00784b74;
    do {
      sVar1 = *psVar9;
      if (0 < sVar1) {
        (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
        DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
        if (99 < DAT_00d3b1c4) {
          DAT_00d3b1c4 = 0;
        }
        local_4 = 0x1f;
        do {
          uVar4 = DAT_00d3b384 >> 4;
          uVar6 = DAT_00d3b384 & 1;
          uVar7 = DAT_00d3b380 >> 4;
          uVar5 = DAT_00d3b380 & 1;
          DAT_00d3b384 = DAT_00d3b384 >> 1;
          DAT_00d3b380 = DAT_00d3b380 >> 1;
          if ((uVar4 & 1) != uVar6) {
            DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
          }
          if ((uVar7 & 1) != uVar5) {
            DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
          }
          local_4 = local_4 + -1;
        } while (local_4 != 0);
        _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
        DAT_00d3b374 = DAT_00d3b384 & 0x7f;
        _DAT_00d3b378 = DAT_00d3b384 & 7;
        _DAT_00d3b36c = DAT_00d3b380 & 7;
        _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
        _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
        iVar3 = FUN_004b4800(1,0x41,(int)sVar1,(int)psVar9[8],0);
        bVar2 = FUN_00562348();
        (&DAT_00bcd5f9)[iVar3 * 0x184] = bVar2 & 0x1f;
        bVar2 = FUN_00562348();
        iVar10 = DAT_00d056f8;
        (&DAT_00bcd631)[iVar3 * 0x184] = bVar2 & 7;
        FUN_004b4220(iVar10,(int)(short)(&DAT_00bcd642)[iVar10 * 0xc2],
                     (int)(short)(&DAT_00bcd644)[iVar10 * 0xc2],
                     *(short *)(&DAT_00bcd61c + iVar10 * 0x184) * 0xf,
                     *(short *)(&DAT_00bcd61e + iVar10 * 0x184) * 0xf,0);
      }
      psVar9 = psVar9 + 1;
    } while ((int)psVar9 < 0x784b84);
  }
  if ((param_3 != 0) && ((DAT_00605a3c != 0 || (DAT_00784cb1 != '\0')))) {
    iVar10 = 1;
    pcVar8 = &DAT_00bcd786;
    do {
      if ((pcVar8[4] != '\0') && (*pcVar8 == '%')) {
        FUN_004b4980(iVar10);
      }
      pcVar8 = pcVar8 + 0x184;
      iVar10 = iVar10 + 1;
    } while ((int)pcVar8 < 0xc8ad42);
    iVar10 = 0;
    do {
      iVar3 = FUN_004b4800(1,0x25,param_1,param_2,0);
      iVar3 = iVar3 * 0x184;
      (&DAT_00bcd630)[iVar3] = 8;
      if (iVar10 == 0) {
        (&DAT_00bcd603)[iVar3] = 3;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 10;
      }
      else if (iVar10 == 1) {
        (&DAT_00bcd603)[iVar3] = 1;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x32;
      }
      else if (iVar10 == 2) {
        (&DAT_00bcd603)[iVar3] = 3;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 100;
      }
      else if (iVar10 == 3) {
        (&DAT_00bcd603)[iVar3] = 2;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x82;
      }
      else if (iVar10 == 4) {
        (&DAT_00bcd603)[iVar3] = 1;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 200;
      }
      else if (iVar10 == 5) {
        (&DAT_00bcd603)[iVar3] = 3;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0xfa;
      }
      else if (iVar10 == 6) {
        (&DAT_00bcd603)[iVar3] = 2;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 400;
      }
      else if (iVar10 == 7) {
        (&DAT_00bcd603)[iVar3] = 3;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x1ae;
      }
      else if (iVar10 == 8) {
        (&DAT_00bcd603)[iVar3] = 2;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 500;
      }
      else if (iVar10 == 9) {
        (&DAT_00bcd603)[iVar3] = 1;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 600;
      }
      else if (iVar10 == 10) {
        (&DAT_00bcd603)[iVar3] = 3;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x46;
      }
      else if (iVar10 == 0xb) {
        (&DAT_00bcd603)[iVar3] = 3;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x2ee;
      }
      else if (iVar10 == 0xc) {
        (&DAT_00bcd603)[iVar3] = 2;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x334;
      }
      else if (iVar10 == 0xd) {
        (&DAT_00bcd603)[iVar3] = 3;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x33e;
      }
      else if (iVar10 == 0xe) {
        (&DAT_00bcd603)[iVar3] = 3;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 900;
      }
      else if (iVar10 == 0xf) {
        (&DAT_00bcd603)[iVar3] = 3;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x3d4;
      }
      else if (iVar10 == 0x10) {
        (&DAT_00bcd603)[iVar3] = 1;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x3f2;
      }
      else if (iVar10 == 0x11) {
        (&DAT_00bcd603)[iVar3] = 2;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x406;
      }
      else if (iVar10 == 0x12) {
        (&DAT_00bcd603)[iVar3] = 0;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x4b0;
      }
      else if (iVar10 == 0x13) {
        (&DAT_00bcd603)[iVar3] = 1;
        *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0x514;
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 < 0x14);
  }
  return;
}
