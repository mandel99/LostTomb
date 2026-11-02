#include "restore_types.h"
#include "town_trade.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_007770f0 (*(undefined4 *)0x007770f0u)
#define DAT_0077de03 (*(undefined4 *)0x0077de03u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e39088 (*(char *)0x00e39088u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5b48 (*(undefined4 *)0x00ea5b48u)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f56b04 (*(int *)0x00f56b04u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern int FUN_004449d0();
extern undefined * FUN_00447840();
extern int * FUN_0044baf0();
extern int FUN_0044c100();
extern int FUN_0044c9a0();

extern void FUN_004cc770();
extern void FUN_004ccc30();
extern void FUN_004cd680();
extern void FUN_004d6160();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern void FUN_004e7db0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00523ea0(void)

{
  undefined1 uVar1;
  short sVar2;
  bool bVar3;
  char cVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined4 uVar11;
  int local_8;
  
  FUN_004e7db0();
  iVar5 = 5 - DAT_00f53f48;
  DAT_00f53f0c = DAT_00e38da8;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x1b;
  if (iVar5 < 5) {
    if (0 < iVar5) {
      FUN_004cd680(iVar5 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar5 * 8,(iVar5 * -2 + 0x28) * 0x10,
                   (iVar5 * -2 + 0x1b) * 0x10,0);
      return;
    }
    FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x1b);
    FUN_0053e590(DAT_00f5681c + 2,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
    DAT_00e20778 = 0;
    FUN_004d62f0(0x34,0x51,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
                 DAT_00e38da4);
    uVar6 = FUN_00447840(DAT_00e20778 + 0x3c + DAT_00f53f0c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0);
    FUN_004cc770(uVar6);
    DAT_00e20778 = 0;
    FUN_004d62f0(0x34,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x2c,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5b48)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,
                 DAT_00e20778 + 0x3c + DAT_00f53f0c,DAT_00f53f08 + 0x2c,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6160(0x34,(int)(&DAT_00ea5b48)[DAT_00eaac28 * 0x1276] / 5 + 0x16,DAT_00f53f0c + 0x3c,
                 DAT_00f53f08 + 0x3c,0x220,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d65b0(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x5a,0x24,0xe,DAT_00e38da8,DAT_00e38da4);
    local_8 = 0;
    iVar5 = FUN_0044baf0(0);
    if (iVar5 != 0) {
      iVar10 = 0;
      do {
        cVar4 = FUN_0044d4c0((const struct RestoreTownRecord *)(uintptr_t)iVar5);
        if (cVar4 != '\0') {
          if (0xd1 < iVar10) break;
          if (*(short *)(iVar5 + 4) == 0x25) {
            iVar7 = FUN_004449d0((int)*(short *)(iVar5 + 0x1c));
            sVar2 = *(short *)(iVar5 + 0xc);
            uVar6 = FUN_0044c9a0();
            FUN_004d63d0(0,DAT_00f53f0c + 0x26,iVar10 + 0x60 + DAT_00f53f08,0x230,0x28,0,
                         DAT_00e38da8,DAT_00e38da4);
            iVar8 = (int)*(short *)(iVar5 + 0x1c);
            bVar3 = false;
            if (((-1 < iVar8) && (iVar8 < 0x32)) && ((&DAT_007770f0)[iVar8 * 0x144] == '\x02')) {
              bVar3 = true;
            }
            FUN_0053e590((uint)bVar3 + DAT_00f56b04,DAT_00f53f0c + 0x32,iVar10 + 0x6a + DAT_00f53f08
                        );
            DAT_00e20778 = 0;
            if (bVar3) {
              uVar11 = 0x50;
            }
            else {
              uVar11 = 0x48;
            }
            FUN_004d62f0(0x34,uVar11,DAT_00f53f0c + 0x50,iVar10 + 0x66 + DAT_00f53f08,DAT_00f56f9c,0
                         ,DAT_00e38da8,DAT_00e38da4);
            if (DAT_00e39088 == '\0') {
              uVar1 = (&DAT_0077de03)[iVar7 * 0x6a];
              uVar11 = 0xc3;
            }
            else {
              uVar1 = (&DAT_0077de03)[iVar7 * 0x6a];
              uVar11 = 0x15;
            }
            FUN_004d62f0(uVar11,uVar1,DAT_00e20778 + 0x50 + DAT_00f53f0c,
                         iVar10 + 0x66 + DAT_00f53f08,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
            DAT_00e20778 = 0;
            if (sVar2 < 0x2e) {
              cVar4 = 'I';
            }
            else {
              cVar4 = (0x58 < sVar2) + 'J';
            }
            FUN_004d62f0(0x34,cVar4,DAT_00f53f0c + 0x50,iVar10 + 0x78 + DAT_00f53f08,DAT_00f56f9c,0,
                         DAT_00e38da8,DAT_00e38da4);
            FUN_004ccc30(8,4,uVar6,DAT_00e20778 + 0x50 + DAT_00f53f0c,iVar10 + 0x78 + DAT_00f53f08,
                         DAT_00f56f9c,0);
          }
          else {
            iVar7 = (int)*(short *)(iVar5 + 0xc);
            iVar8 = (int)*(short *)(iVar5 + 4);
            switch((int)(iVar8)) {
            case 0x18: ;
            case 0x19: ;
            case 0x1a: ;
            case 0x1e: ;
            case 0x23: ;
              break;
default: ;
              if (((iVar8 != 0x1c) && (iVar8 != 10)) && (iVar8 != 0x24)) {
                iVar7 = iVar7 * 100;
              }
            }
            FUN_004d63d0(0,DAT_00f53f0c + 0x26,iVar10 + 0x60 + DAT_00f53f08,0x230,0x28,0,
                         DAT_00e38da8,DAT_00e38da4);
            if (iVar8 == 0x24) {
              iVar9 = DAT_00f571fc + 0x15;
            }
            else {
              iVar9 = DAT_00f571fc + iVar8;
            }
            FUN_0053e590(iVar9,DAT_00f53f0c + 0x28,iVar10 + 100 + DAT_00f53f08);
            DAT_00e20778 = 0;
            FUN_004d6860(iVar7,0x40,&DAT_0057c048,DAT_00f53f0c + 0x42,iVar10 + 0x66 + DAT_00f53f08,
                         DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
            switch((int)(iVar8)) {
            case 0x18: ;
            case 0x19: ;
            case 0x1a: ;
            case 0x1e: ;
            case 0x23: ;
switchD_00524425_caseD_18: ;
              iVar9 = iVar8 + 0x36;
              break;
default: ;
              iVar9 = iVar8;
              if (((iVar8 == 0x1c) || (iVar8 == 10)) && (1 < iVar7)) goto switchD_00524425_caseD_18;
            }
            FUN_004d62f0(0x17,iVar9,DAT_00e20778 + 0x42 + DAT_00f53f0c,iVar10 + 0x66 + DAT_00f53f08,
                         DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
            uVar6 = FUN_0044c100(DAT_00f53f0c + 0x136,iVar10 + 0x66 + DAT_00f53f08,DAT_00f56f9c,0);
            FUN_004ccc30(8,4,uVar6);
            FUN_004d62f0(0xc,2,DAT_00e20778 + 0x136 + DAT_00f53f0c,iVar10 + 0x66 + DAT_00f53f08,
                         DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
            DAT_00e20778 = 0;
            if (iVar8 == 0x24) {
              FUN_004d6860(*(undefined4 *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048
                           ,DAT_00f53f0c + 0x28,iVar10 + 0x78 + DAT_00f53f08,DAT_00f56f9c,0,
                           DAT_00e38da8,DAT_00e38da4);
              FUN_004d62f0(0x34,0x2c,DAT_00e20778 + 0x28 + DAT_00f53f0c,iVar10 + 0x78 + DAT_00f53f08
                           ,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
              if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < iVar7) {
                FUN_004d62f0(0x34,0x30,DAT_00e20778 + 0x50 + DAT_00f53f0c,
                             iVar10 + 0x78 + DAT_00f53f08,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
              }
              else {
                FUN_004d62f0(0x34,0x2f,DAT_00e20778 + 0x50 + DAT_00f53f0c,
                             iVar10 + 0x78 + DAT_00f53f08,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
              }
            }
            else {
              iVar9 = (&DAT_00ea6160)[DAT_00eaac28 * 0x1276 + iVar8];
              switch((int)(iVar8)) {
              case 0x18: ;
              case 0x19: ;
              case 0x1a: ;
              case 0x1e: ;
              case 0x23: ;
switchD_00524674_caseD_18: ;
                iVar9 = iVar9 / 100;
                break;
default: ;
                if ((iVar8 == 0x1c) || (iVar8 == 10)) goto switchD_00524674_caseD_18;
              }
              FUN_004d6860(iVar9,0x40,&DAT_0057c048,DAT_00f53f0c + 0x28,iVar10 + 0x78 + DAT_00f53f08
                           ,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
              FUN_004d62f0(0x34,0x2b,DAT_00e20778 + 0x28 + DAT_00f53f0c,iVar10 + 0x78 + DAT_00f53f08
                           ,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
              if (iVar9 < iVar7) {
                uVar6 = 0x30;
              }
              else {
                uVar6 = 0x2f;
              }
              FUN_004d62f0(0x34,uVar6,DAT_00e20778 + 0x50 + DAT_00f53f0c,
                           iVar10 + 0x78 + DAT_00f53f08,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
            }
          }
          local_8 = local_8 + 1;
          iVar10 = iVar10 + 0x2a;
        }
        iVar5 = FUN_0044baf0(iVar5);
      } while (iVar5 != 0);
      if (local_8 != 0) {
        return;
      }
    }
    FUN_004d6160(0x34,0x15,DAT_00f53f0c + 0x40,DAT_00f53f08 + 0xa0,0x200,DAT_00f56f9c,0,DAT_00e38da8
                 ,DAT_00e38da4);
  }
  return;
}
