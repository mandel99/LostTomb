#include "restore_types.h"
#define DAT_00d3b5dc (*(int32_t *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(int32_t *)0x00d3b5e0u)
#define DAT_00f5698c (*(int32_t *)0x00f5698cu)
extern int FUN_0053e590();

void FUN_00536c30(int param_1,int param_2,int param_3,uint param_4,uint param_5,int param_6)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  bool bVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  
  iVar7 = (int)(param_4 + ((int)param_4 >> 0x1f & 0xfU)) >> 4;
  uVar1 = param_4 & 0x8000000f;
  bVar9 = uVar1 == 0;
  if ((int)uVar1 < 0) {
    bVar9 = (uVar1 - 1 | 0xfffffff0) == 0xffffffff;
  }
  if (!bVar9) {
    iVar7 = iVar7 + 1;
  }
  iVar3 = (int)(((int)param_5 >> 0x1f & 0xfU) + param_5) >> 4;
  uVar1 = param_5 & 0x8000000f;
  bVar9 = uVar1 == 0;
  if ((int)uVar1 < 0) {
    bVar9 = (uVar1 - 1 | 0xfffffff0) == 0xffffffff;
  }
  if (!bVar9) {
    iVar3 = iVar3 + 1;
  }
  iVar4 = iVar7 * 0x10 - param_4;
  iVar5 = iVar3 * 0x10 - param_5;
  iVar6 = DAT_00f5698c;
  if (param_1 == 0) {
    if (param_6 != 0) {
      iVar6 = DAT_00f5698c + 8;
    }
  }
  else if (param_6 != 0) {
    iVar6 = DAT_00f5698c + 8;
  }
  param_6 = 0;
  if (0 < iVar3) {
    do {
      iVar8 = 0;
      if (0 < iVar7) {
        iVar2 = param_6 * 0x10 + param_3;
        do {
          DAT_00d3b5e0 = iVar8 * 0x10 + param_2;
          DAT_00d3b5dc = iVar2;
          iVar11 = DAT_00d3b5e0;
          iVar12 = iVar2;
          if (param_6 == 0) {
            iVar10 = iVar6;
            if (iVar8 != 0) {
              if (iVar8 < iVar7 + -1) {
                iVar10 = iVar6 + 1;
              }
              else {
                iVar10 = iVar6 + 2;
                iVar11 = DAT_00d3b5e0 - iVar4;
              }
            }
LAB_00536d81: ;
            FUN_0053e590(iVar10,iVar11,iVar12);
          }
          else {
            if (iVar3 + -1 <= param_6) {
              if (iVar8 == 0) {
                iVar10 = iVar6 + 6;
                iVar12 = iVar2 - iVar5;
              }
              else if (iVar8 < iVar7 + -1) {
                iVar10 = iVar6 + 5;
                iVar12 = iVar2 - iVar5;
              }
              else {
                iVar10 = iVar6 + 4;
                iVar11 = DAT_00d3b5e0 - iVar4;
                iVar12 = iVar2 - iVar5;
              }
              goto LAB_00536d81;
            }
            if (iVar8 == 0) {
              iVar10 = iVar6 + 7;
              goto LAB_00536d81;
            }
            if (iVar7 + -1 <= iVar8) {
              iVar10 = iVar6 + 3;
              iVar11 = DAT_00d3b5e0 - iVar4;
              goto LAB_00536d81;
            }
          }
          iVar8 = iVar8 + 1;
        } while (iVar8 < iVar7);
      }
      param_6 = param_6 + 1;
    } while (param_6 < iVar3);
  }
  return;
}
