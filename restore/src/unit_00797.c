#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00bc9690 (*(undefined *)0x00bc9690u)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
#define DAT_00bca30c (*(char *)0x00bca30cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00401480();
extern void FUN_0040d890();
extern void FUN_00416fb0();
extern void FUN_0043fd00();
extern void FUN_0046ac10();
extern int FUN_00475b50();
extern void FUN_0047b1a0();
extern void FUN_0047c480();
extern void FUN_00486e70();
extern void FUN_004bfee0();
extern void FUN_004e6660();
extern void FUN_004e9170();
extern void FUN_00517370();
extern void FUN_00517660();
extern void FUN_00545720();
extern void FUN_0054f450();
extern void FUN_00555e60();
extern void FUN_00558bc0();

void FUN_004525f0(void)

{
  char cVar1;
  short sVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  int iVar6;
  int iVar7;
  short *psVar8;
  
  iVar7 = 1;
  bVar3 = false;
  bVar4 = false;
  bVar5 = false;
  if (0 < DAT_0078d790) {
    psVar8 = &DAT_00799af0;
    do {
      if ((char)psVar8[-8] == '\x03') {
        *(undefined1 *)(psVar8 + -8) = 1;
      }
      cVar1 = (char)psVar8[-8];
      if ((cVar1 != '\x01') || (*(char *)((int)psVar8 + -0xb) == '\0')) {
        if ((cVar1 == '\x02') || (cVar1 == '\x06')) {
          sVar2 = *psVar8;
          if (sVar2 == 0x3b) {
            bVar3 = true;
          }
          if (sVar2 == 0xac) {
            bVar3 = true;
          }
          if (sVar2 == 0xad) {
            bVar3 = true;
          }
          if (sVar2 == 0x3a) {
            bVar3 = true;
          }
          if (sVar2 == 0xaa) {
            bVar3 = true;
          }
          if (sVar2 == 0xab) {
            FUN_004bfee0(iVar7);
            FUN_0046ac10(iVar7);
            bVar3 = true;
            bVar4 = true;
          }
          else {
            if (sVar2 == 200) {
              bVar3 = true;
            }
            if (sVar2 == 0xc9) {
              bVar3 = true;
            }
            if (sVar2 != 0xca) {
              if ((((sVar2 == 0xd1) || (sVar2 == 0x1e)) || (sVar2 == 0x1f)) || (sVar2 == 0x21)) {
                iVar6 = FUN_00475b50(iVar7);
                if (iVar6 == iVar7) {
                  FUN_0043fd00(iVar7,(int)*psVar8);
                  bVar4 = true;
                }
              }
              else {
                if (sVar2 == 0xb7) {
                  bVar5 = true;
                }
                if (sVar2 == 0xe1) {
                  FUN_0040d890(iVar7);
                }
                else if (sVar2 == 0xe6) {
                  FUN_00401480(iVar7);
                }
                else if (sVar2 == 0xe3) {
                  FUN_00416fb0(iVar7);
                }
                else if (sVar2 == 0xe4) {
                  FUN_004e6660(iVar7);
                }
                else if (sVar2 == 0xe5) {
                  FUN_00545720(iVar7);
                }
                else if (sVar2 == 0xea) {
                  FUN_00558bc0(iVar7);
                }
                else if (sVar2 == 0xeb) {
                  FUN_00555e60(iVar7);
                }
                else if (sVar2 == 0xec) {
                  FUN_0054f450(iVar7);
                }
                else {
                  FUN_0047c480(iVar7,(int)psVar8[-5],(int)psVar8[-4]);
                }
                bVar4 = true;
                if (((((DAT_00bc9690 == 9) && (*psVar8 == 0xe6)) ||
                     ((sVar2 = *psVar8, sVar2 == 0xe3 || ((sVar2 == 0xe1 || (sVar2 == 0xe4)))))) ||
                    (sVar2 == 0xe5)) || (((sVar2 == 0xea || (sVar2 == 0xeb)) || (sVar2 == 0xec)))) {
                  DAT_00bc9694 = 0;
                }
              }
              goto LAB_00452854;
            }
            FUN_004bfee0(iVar7);
            FUN_0046ac10(iVar7);
            bVar3 = true;
            bVar4 = true;
          }
        }
        else {
          if (cVar1 == '\x04') {
            if (*(char *)((int)psVar8 + -0xb) != '\0') {
              FUN_004e9170(DAT_00eaac28,(int)psVar8[6]);
            }
          }
          else if (cVar1 != '\x05') goto LAB_0045285d;
LAB_00452854: ;
          FUN_0046ac10(iVar7);
        }
      }
LAB_0045285d: ;
      iVar7 = iVar7 + 1;
      psVar8 = psVar8 + 0x84;
    } while (iVar7 <= DAT_0078d790);
    if (bVar5) {
      DAT_00bca30c = 1;
      FUN_00486e70();
    }
    if (bVar3) {
      FUN_0047b1a0();
    }
    if (bVar4) {
      FUN_00517370();
      FUN_00517660();
    }
  }
  return;
}
