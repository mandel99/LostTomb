#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a9c (*(undefined4 *)0x00799a9cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern int FUN_0047cbd0();
extern undefined4 FUN_004806c0();
extern undefined4 FUN_0051de70();
extern int FUN_0051e720();

void FUN_0043f060(int param_1,char param_2,int param_3)

{
  undefined1 uVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int *piVar11;
  int iVar12;
  int local_fa0 [1000];
  
  iVar10 = param_1 * 0x108;
  iVar7 = (&DAT_00799a9c)[param_1 * 0x42];
  if (0 < iVar7) {
    sVar2 = *(short *)(&DAT_007999e0 + iVar10);
    iVar9 = (int)*(short *)((int)&DAT_007999dc + iVar10 + 2);
    FUN_0047cbd0(iVar9,(int)sVar2,0);
    iVar6 = _DAT_00d3b4e4;
    iVar12 = _DAT_00d3b4e0;
    piVar11 = local_fa0;
    for (iVar8 = 0xfa; iVar8 != 0; iVar8 = iVar8 + -1) {
      *piVar11 = -1;
      piVar11 = piVar11 + 1;
    }
    iVar7 = iVar7 * 0x108;
    sVar3 = *(short *)((int)&DAT_007999dc + iVar7 + 2);
    sVar4 = *(short *)(&DAT_007999e0 + iVar7);
    uVar1 = (&DAT_007999db)[iVar10];
    FUN_004806c0(iVar9,(int)sVar2,uVar1);
    iVar8 = _DAT_00d3b4e4;
    iVar7 = _DAT_00d3b4e0;
    FUN_004806c0((int)sVar3,(int)sVar4,uVar1);
    iVar9 = _DAT_00d3b4e4;
    iVar10 = _DAT_00d3b4e0;
    FUN_0051de70(iVar8,iVar7,_DAT_00d3b4e4,_DAT_00d3b4e0);
    iVar10 = FUN_0051e720(0x3e9,iVar8,iVar7,iVar9,iVar10,0,local_fa0);
    _DAT_00d3b4e0 = iVar12;
    _DAT_00d3b4e4 = iVar6;
    if (iVar10 != 0) {
      iVar7 = DAT_00bca2d8 + iVar7 * 0xe4;
      iVar12 = iVar8 + iVar7;
      if (param_2 == '\0') {
        uVar5 = (&DAT_00af598c)[iVar12];
        *(undefined1 *)((int)&DAT_00ee0c50 + iVar12) = 0xff;
        (&DAT_00af598c)[iVar12] = uVar5 & 0xffffffbf;
      }
      else {
        (&DAT_00af598c)[iVar12] = (&DAT_00af598c)[iVar12] | 0x40;
        if (param_3 == 1) {
          *(undefined1 *)((int)&DAT_00ee0c50 + iVar12) = 1;
        }
        else {
          *(undefined1 *)((int)&DAT_00ee0c50 + iVar12) = 3;
        }
      }
      if (0 < iVar10) {
        piVar11 = local_fa0;
        do {
          iVar12 = *piVar11;
          if (iVar12 == 0) {
            iVar7 = iVar7 + -0xe4;
          }
          else if (iVar12 == 2) {
            iVar8 = iVar8 + 1;
          }
          else if (iVar12 == 4) {
            iVar7 = iVar7 + 0xe4;
          }
          else {
            iVar8 = iVar8 + -1;
          }
          iVar12 = iVar7 + iVar8;
          if (param_2 == '\0') {
            (&DAT_00af598c)[iVar12] = (&DAT_00af598c)[iVar12] & 0xffffffbf;
            *(undefined1 *)((int)&DAT_00ee0c50 + iVar12) = 0xff;
          }
          else {
            (&DAT_00af598c)[iVar12] = (&DAT_00af598c)[iVar12] | 0x40;
            if (param_3 == 1) {
              *(undefined1 *)((int)&DAT_00ee0c50 + iVar12) = 1;
            }
            else {
              *(undefined1 *)((int)&DAT_00ee0c50 + iVar12) = 3;
            }
          }
          piVar11 = piVar11 + 1;
          iVar10 = iVar10 + -1;
        } while (iVar10 != 0);
      }
    }
  }
  return;
}
