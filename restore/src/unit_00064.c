#include "restore_types.h"
#define DAT_005d2a18 (*(undefined *)0x005d2a18u)
#define DAT_005d2a1c (*(undefined *)0x005d2a1cu)
#define DAT_005d2a20 (*(undefined *)0x005d2a20u)
#define DAT_0077590c (*(undefined *)0x0077590cu)
#define DAT_00775910 (*(undefined *)0x00775910u)
#define DAT_0077591c (*(undefined *)0x0077591cu)
#define DAT_00775920 (*(undefined *)0x00775920u)
#define DAT_00775924 (*(undefined *)0x00775924u)
#define DAT_00775928 (*(undefined *)0x00775928u)
#define DAT_0077de6a (*(undefined4 *)0x0077de6au)
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_0077f749 (*(undefined4 *)0x0077f749u)
#define DAT_0077f74c (*(undefined4 *)0x0077f74cu)
#define DAT_0077f74e (*(undefined4 *)0x0077f74eu)
#define DAT_0077f750 (*(undefined4 *)0x0077f750u)
#define DAT_0077f752 (*(undefined4 *)0x0077f752u)
#define DAT_0077f754 (*(undefined4 *)0x0077f754u)
#define DAT_0077f756 (*(undefined4 *)0x0077f756u)
#define DAT_0077f75c (*(undefined4 *)0x0077f75cu)
#define DAT_0077f75e (*(undefined4 *)0x0077f75eu)
#define DAT_0077f760 (*(undefined4 *)0x0077f760u)
#define DAT_00784cb4 (*(undefined *)0x00784cb4u)
#define DAT_00e38d98 (*(undefined *)0x00e38d98u)
#define DAT_00e38d9c (*(undefined *)0x00e38d9cu)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38e00 (*(char *)0x00e38e00u)
#define DAT_00e38e03 (*(undefined1 *)0x00e38e03u)
#define DAT_00e38e04 (*(undefined1 *)0x00e38e04u)
#define DAT_00e38e30 (*(undefined *)0x00e38e30u)
#define DAT_00e38e34 (*(undefined *)0x00e38e34u)
#define DAT_00f56a0c (*(undefined *)0x00f56a0cu)
#define DAT_00f56a14 (*(undefined *)0x00f56a14u)
#define DAT_010d6a0c (*(undefined4 *)0x010d6a0cu)
#define DAT_010d6a0e (*(undefined4 *)0x010d6a0eu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
#define _DAT_005d2a2c (*(undefined4 *)0x005d2a2cu)
extern void FUN_004264d0();
extern void FUN_004426c0();
extern void FUN_00444720();
extern void FUN_0051f780();

void FUN_00407620(int param_1,int param_2)

{
  short sVar1;
  bool bVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  short sVar6;
  char *pcVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  short sVar13;
  
  iVar5 = DAT_00e38e34;
  iVar4 = DAT_00e38e30;
  cVar3 = DAT_00e38e04;
  iVar12 = DAT_00775924;
  iVar10 = DAT_00775920;
  if (((DAT_00e38e00 != '\0') || (DAT_00e38e04 != '\0')) || (DAT_00e38e03 != '\0')) {
    if ((DAT_005d2a18 <= DAT_00e38e34) && (DAT_00e38e34 < DAT_00e38d9c + DAT_005d2a18 * -2)) {
      if ((DAT_005d2a1c <= DAT_00e38e30) &&
         (DAT_00e38e30 < (DAT_00e38d98 - DAT_005d2a20) - DAT_005d2a1c)) {
        bVar2 = false;
        iVar9 = param_1 * 0x62;
        if ((&DAT_0077f749)[iVar9] == '\0') {
          (&DAT_0077f748)[iVar9] = (char)DAT_00775920;
          (&DAT_0077f754)[param_1 * 0x31] = (short)iVar12;
          if (iVar10 == 1) {
            pcVar7 = &DAT_0077de6a;
            iVar10 = 1;
            do {
              if (*pcVar7 != '\0') {
                iVar10 = iVar10 + 1;
              }
              pcVar7 = pcVar7 + 0x6a;
            } while ((int)pcVar7 < 0x77f742);
            if (0x3c < iVar10) {
              DAT_00775928 = 4;
              FUN_004264d0(0x22,0x0051f780u,1);
              return;
            }
            bVar2 = true;
          }
          else if (iVar10 == 0) {
            iVar12 = iVar12 - DAT_00f56a0c;
            if (DAT_00f56a14 <= iVar12) {
              iVar12 = DAT_00f56a14;
            }
            *(short *)(&DAT_0077f756 + iVar9) = (short)iVar12;
          }
        }
        DAT_0077591c = param_1;
        (&DAT_0077f749)[iVar9] = 1;
        if (cVar3 != '\0') {
          (&DAT_0077f749)[iVar9] = 2;
        }
        uVar8 = (uint)(ushort)(&DAT_0077f754)[param_1 * 0x31];
        uVar11 = uVar8;
        if (DAT_0126517c <= (int)uVar8) {
          uVar11 = 0;
        }
        iVar10 = (int)(short)(&DAT_010d6a0c)[uVar11 * 0x20];
        if (DAT_0126517c <= (int)uVar8) {
          uVar8 = 0;
        }
        iVar12 = (int)(short)(&DAT_010d6a0e)[uVar8 * 0x20];
        if ((&DAT_0077f748)[iVar9] == '\x02') {
          iVar10 = 0x5a;
          iVar12 = 0x14;
        }
        sVar6 = (short)DAT_0077590c;
        sVar1 = (short)DAT_005d2a1c;
        if (DAT_00784cb4 == 0) {
          (&DAT_0077f74c)[param_1 * 0x31] =
               (((short)iVar5 - (short)(iVar10 / 2)) - (short)DAT_005d2a18) + (short)DAT_00775910;
          (&DAT_0077f74e)[param_1 * 0x31] = (((short)iVar4 - (short)(iVar12 / 2)) - sVar1) + sVar6;
        }
        else {
          sVar13 = (((short)iVar5 - (short)(iVar10 / 2)) - (short)DAT_005d2a18) +
                   (short)DAT_00775910;
          (&DAT_0077f75c)[param_1 * 0x31] = sVar13;
          sVar6 = (((short)iVar4 - (short)(iVar12 / 2)) - sVar1) + sVar6;
          (&DAT_0077f75e)[param_1 * 0x31] = sVar6;
          if (param_2 == 0) {
            (&DAT_0077f74c)[param_1 * 0x31] = sVar13;
            (&DAT_0077f74e)[param_1 * 0x31] = sVar6;
          }
        }
        (&DAT_0077f750)[param_1 * 0x31] = (short)iVar10;
        (&DAT_0077f752)[param_1 * 0x31] = (short)iVar12;
        if (bVar2) {
          (&DAT_0077f760)[iVar9] = 3;
          FUN_004426c0();
          FUN_00444720();
        }
        _DAT_005d2a2c = 0;
        DAT_00e38dca = 1;
      }
    }
  }
  return;
}
