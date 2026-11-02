#include "restore_types.h"
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bca2c4 (*(undefined *)0x00bca2c4u)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00d3a6cc (*(undefined *)0x00d3a6ccu)
#define DAT_00d3a6d0 (*(undefined *)0x00d3a6d0u)
#define DAT_00d3a6d4 (*(undefined *)0x00d3a6d4u)
#define DAT_00d3a6d8 (*(undefined *)0x00d3a6d8u)
#define DAT_00d3b51c (*(undefined *)0x00d3b51cu)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38e30 (*(undefined *)0x00e38e30u)
#define DAT_00e38e34 (*(undefined *)0x00e38e34u)
#define DAT_00e92dfc (*(undefined *)0x00e92dfcu)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
extern int FUN_004cbf30();
extern void FUN_004cc110();
extern void FUN_004cc770();
extern void FUN_004cd680();
extern void FUN_004cd9d0();

void FUN_004ca9a0(void)

{
  byte bVar1;
  undefined2 uVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  uVar6 = 0x30;
  iVar3 = (int)*(short *)(&DAT_00b350dc + DAT_00bca2c4 * 2);
  if (iVar3 != 0) {
    while ((&DAT_00bcd602)[iVar3 * 0x184] != '$') {
      iVar3 = (int)*(short *)(&DAT_00bcd600 + iVar3 * 0x184);
      if (iVar3 == 0) {
        return;
      }
    }
    bVar1 = (&DAT_00bcd603)[iVar3 * 0x184];
    if (bVar1 == 1) {
      uVar6 = 0x31;
      iVar3 = 3;
    }
    else if (bVar1 == 2) {
      iVar3 = 5;
    }
    else if (bVar1 == 3) {
      iVar3 = 6;
    }
    else if (bVar1 == 0xc) {
      iVar3 = 0x12;
    }
    else if (bVar1 == 0xd) {
      iVar3 = 0x13;
    }
    else if ((bVar1 < 0xe) || (0x15 < bVar1)) {
      if ((bVar1 < 0x16) || (0x19 < bVar1)) {
        if ((bVar1 < 0x1a) || (0x1d < bVar1)) {
          if ((bVar1 < 0x1e) || (0x20 < bVar1)) {
            if ((bVar1 < 0x21) || (0x22 < bVar1)) {
              uVar4 = (uint)bVar1;
              if ((int)(uVar4 - 0x32) < 8) {
                iVar3 = uVar4 - 0x28;
              }
              else {
                iVar3 = uVar4 - 10;
              }
            }
            else {
              iVar3 = bVar1 + 0x1a;
            }
          }
          else {
            iVar3 = bVar1 + 0x1a;
          }
        }
        else {
          iVar3 = bVar1 + 0xd;
        }
      }
      else {
        iVar3 = bVar1 + 9;
      }
    }
    else {
      iVar3 = bVar1 + 9;
    }
    FUN_004cc110(uVar6,iVar3);
    uVar6 = DAT_00e20768;
    iVar3 = FUN_004cbf30(DAT_00e20768,DAT_00f56f84);
    uVar2 = DAT_00d3b51c;
    DAT_00d3a6d0 = iVar3 + 0x14;
    DAT_00d3a6cc = 0x14;
    if (DAT_00e38e34 < iVar3 + 0x28) {
      DAT_00d3a6d8 = DAT_00e38e34 + 0x14;
    }
    else {
      DAT_00d3a6d8 = (DAT_00e38e34 - DAT_00d3a6d0) + -0x14;
    }
    if (DAT_00e38e30 < 200) {
      DAT_00d3a6d4 = DAT_00e38e30 + 10;
    }
    else {
      DAT_00d3a6d4 = DAT_00e38e30 + -0x20;
    }
    if (DAT_00e92dfc == 2) {
      DAT_00d3b51c = 0x3922;
      uVar7 = 0xbb88;
      uVar5 = 0x58a0;
    }
    else {
      DAT_00d3b51c = 0x1c82;
      uVar7 = 0x5dc8;
      uVar5 = 0x2c40;
    }
    FUN_004cd680(DAT_00d3a6d8,DAT_00d3a6d4,DAT_00d3a6d0,0x14,uVar5);
    FUN_004cd9d0(DAT_00d3a6d8 + 1,DAT_00d3a6d4 + 1,DAT_00d3a6d0 + -2,DAT_00d3a6cc + -2,uVar7);
    DAT_00e20778 = 0;
    FUN_004cc770(uVar6,DAT_00d3a6d8 + 5,DAT_00d3a6d4 + 5,DAT_00f56f84,(int)DAT_00d3b51c);
    DAT_00d3b51c = uVar2;
  }
  return;
}
