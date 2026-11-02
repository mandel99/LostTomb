#include "restore_types.h"
#define DAT_005ccd1c (*(undefined4 *)0x005ccd1cu)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_00799a08 (*(undefined4 *)0x00799a08u)
#define DAT_00799a1b (*(undefined4 *)0x00799a1bu)
#define DAT_00799a1e (*(undefined4 *)0x00799a1eu)
#define DAT_00799a20 (*(undefined4 *)0x00799a20u)
#define DAT_00799a22 (*(undefined4 *)0x00799a22u)
#define DAT_00799a24 (*(undefined4 *)0x00799a24u)
#define DAT_00799a25 (*(undefined4 *)0x00799a25u)
#define DAT_00799a94 (*(undefined4 *)0x00799a94u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f56c54 (*(undefined *)0x00f56c54u)
#define DAT_00f56c5c (*(undefined *)0x00f56c5cu)
extern void FUN_0043fd00();
extern int FUN_0046a400();
extern void FUN_0046ac50();
extern int FUN_0046d0f0();
extern void FUN_0047bf50();
extern void FUN_0047c480();
extern void FUN_004c04a0();
extern void FUN_004e9170();
extern void FUN_005178b0();

void FUN_0046d100(int param_1,undefined1 param_2)

{
  char cVar1;
  short sVar2;
  bool bVar3;
  bool bVar4;
  byte bVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  int *piVar10;
  int iVar11;
  int iVar12;
  
  bVar4 = false;
  iVar12 = param_1 * 0x108;
  if ((&DAT_007999e8)[param_1 * 0x84] != 0xca) {
    *(undefined2 *)(&DAT_00799a20 + iVar12) = 0;
    (&DAT_00799a1e)[param_1 * 0x84] = 0;
    DAT_00bc9694 = 0;
    if ((*(char *)((int)&DAT_007999dc + iVar12 + 1) != '\0') &&
       ((&DAT_007999f4)[param_1 * 0x84] != 0)) {
      FUN_004e9170(DAT_00eaac28,(int)(short)(&DAT_007999f4)[param_1 * 0x84]);
    }
    (&DAT_007999f4)[param_1 * 0x84] = 0;
    *(undefined1 *)((int)&DAT_007999dc + iVar12 + 1) = 0;
    (&DAT_00799a1b)[iVar12] = 0;
    (&DAT_007999f8)[param_1 * 0x84] = 0;
    FUN_0046ac50(param_1);
    sVar2 = (&DAT_007999e8)[param_1 * 0x84];
    if (((((sVar2 == 0x1f) || (sVar2 == 0x1e)) || (sVar2 == 0x21)) || (sVar2 == 0xd1)) &&
       ((iVar6 = FUN_0046d0f0(param_1), 0 < iVar6 && (iVar6 == param_1)))) {
      FUN_0043fd00(iVar6,(int)(short)(&DAT_007999e8)[iVar6 * 0x84]);
    }
    sVar2 = (&DAT_007999e8)[param_1 * 0x84];
    if (((sVar2 == 0x4b) || (sVar2 == 0x4c)) ||
       ((sVar2 == 0x4a || (((sVar2 == 0xb5 || (sVar2 == 0xb6)) || (sVar2 == 0xc0)))))) {
      bVar4 = true;
    }
    if ((*(char *)((int)&DAT_007999dc + iVar12 + 1) == '\0') ||
       (bVar3 = true, 1 < (short)(&DAT_007999ea)[param_1 * 0x84])) {
      bVar3 = false;
    }
    cVar1 = (&DAT_007999db)[iVar12];
    if (cVar1 == '\x02') {
      uVar9 = 4;
    }
    else if (cVar1 == '\x03') {
      uVar9 = 9;
    }
    else if (cVar1 == '\x04') {
      uVar9 = 0x10;
    }
    else if (cVar1 == '\x05') {
      uVar9 = 0x19;
    }
    else {
      uVar9 = (cVar1 != '\x06') - 1 & 0x24;
    }
    FUN_0047c480(param_1,(int)*(short *)((int)&DAT_007999dc + iVar12 + 2),
                 (int)*(short *)(&DAT_007999e0 + iVar12));
    if ((*(byte *)(&DAT_00af598c + *(int *)(&DAT_007999e4 + iVar12)) & 4) == 0) {
      (&DAT_007999e8)[param_1 * 0x84] = 99;
      *(undefined2 *)(&DAT_00799a08 + iVar12) = 0;
      *(ushort *)(&DAT_00799a22 + iVar12) = ((byte)(&DAT_00799a25)[iVar12] & 0xff07) + 1;
      (&DAT_00799a24)[iVar12] = 1;
      (&DAT_007999db)[iVar12] = 1;
      *(undefined1 *)((int)&DAT_00799a94 + iVar12 + 1) = param_2;
      bVar5 = (&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar12)] & 3;
      iVar6 = DAT_00f56c54;
      if ((!bVar3) &&
         (iVar6 = DAT_00f56c5c, ((&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar12)] & 3) != 0)) {
        if (bVar5 == 1) {
          iVar6 = DAT_00f56c5c + 9;
        }
        else if (bVar5 == 2) {
          iVar6 = DAT_00f56c5c + 0x12;
        }
        else {
          iVar6 = DAT_00f56c5c + 0x1b;
        }
      }
      FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar12 + 2),
                   (int)*(short *)(&DAT_007999e0 + iVar12),1,1,iVar6,8);
    }
    else {
      (&DAT_007999d8)[iVar12] = 5;
    }
    if (1 < uVar9) {
      piVar10 = &DAT_005ccd1c;
      iVar6 = uVar9 - 1;
      do {
        iVar8 = piVar10[-1] + (int)*(short *)((int)&DAT_007999dc + iVar12 + 2);
        if ((*(byte *)(&DAT_00af598c +
                      DAT_00bca2d8 + ((int)*(short *)(&DAT_007999e0 + iVar12) + *piVar10) * 0xe4 +
                      iVar8) & 4) == 0) {
          iVar7 = FUN_0046a400(DAT_00eaac28,99,iVar8,
                               (int)*(short *)(&DAT_007999e0 + iVar12) + *piVar10,0,0);
          iVar11 = iVar7 * 0x108;
          bVar5 = (&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar11)] & 3;
          iVar8 = DAT_00f56c54;
          if ((!bVar3) &&
             (iVar8 = DAT_00f56c5c, ((&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar11)] & 3) != 0)) {
            if (bVar5 == 1) {
              iVar8 = DAT_00f56c5c + 9;
            }
            else if (bVar5 == 2) {
              iVar8 = DAT_00f56c5c + 0x12;
            }
            else {
              iVar8 = DAT_00f56c5c + 0x1b;
            }
          }
          FUN_0047bf50(iVar7,(int)*(short *)((int)&DAT_007999dc + iVar11 + 2),
                       (int)*(short *)(&DAT_007999e0 + iVar11),1,1,iVar8,8);
          *(ushort *)(&DAT_00799a22 + iVar11) = ((byte)(&DAT_00799a25)[iVar11] & 0xff07) + 1;
          *(undefined2 *)(&DAT_00799a08 + iVar11) = 0;
          (&DAT_00799a24)[iVar11] = 1;
          *(undefined1 *)((int)&DAT_00799a94 + iVar11 + 1) = param_2;
        }
        piVar10 = piVar10 + 2;
        iVar6 = iVar6 + -1;
      } while (iVar6 != 0);
    }
    if (bVar4) {
      FUN_005178b0();
    }
    return;
  }
  FUN_004c04a0(param_1);
  return;
}
