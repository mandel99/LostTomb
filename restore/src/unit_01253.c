#include "restore_types.h"
#define DAT_00bc96e4 (*(undefined *)0x00bc96e4u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca310 (*(undefined *)0x00bca310u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_004806c0();
extern undefined4 FUN_0051de70();
extern int FUN_0051e720();

undefined4
FUN_00486a30(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,int param_7,
            undefined4 param_8,int *param_9)

{
  bool bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  
  bVar1 = false;
  iVar6 = param_1;
  iVar9 = param_2;
  if (DAT_00bca2e0 != 0) {
    if (DAT_00bca2e0 == 6) {
      iVar9 = param_2 + -1;
    }
    else {
      iVar6 = param_1 + -1;
      if (DAT_00bca2e0 == 4) {
        iVar9 = param_2 + -1;
      }
    }
  }
  iVar6 = FUN_004806c0(iVar6,iVar9,param_5);
  uVar4 = _DAT_00d3b4e4;
  uVar2 = _DAT_00d3b4e0;
  if (iVar6 != 0) {
    iVar6 = param_3;
    iVar9 = param_4;
    if ((DAT_00bca310 == 0) && (DAT_00bca2e0 != 0)) {
      if (DAT_00bca2e0 == 6) {
        iVar9 = param_4 + -1;
      }
      else {
        iVar6 = param_3 + -1;
        if (DAT_00bca2e0 == 4) {
          iVar9 = param_4 + -1;
        }
      }
    }
    iVar6 = FUN_004806c0(iVar6,iVar9,param_5);
    uVar5 = _DAT_00d3b4e4;
    uVar3 = _DAT_00d3b4e0;
    if (iVar6 != 0) {
      FUN_0051de70(uVar4,uVar2,_DAT_00d3b4e4,_DAT_00d3b4e0);
      iVar6 = FUN_0051e720(0x3e9,uVar4,uVar2,uVar5,uVar3,0,param_8);
      *param_9 = iVar6;
      iVar9 = 0;
      param_5 = 0;
      if (-1 < DAT_00bc96e4 * 2) {
        iVar10 = 0;
        do {
          iVar11 = -param_5;
          iVar12 = param_5;
          if (-param_5 == param_5 || iVar11 < param_5) {
            do {
              iVar7 = iVar11;
              iVar8 = iVar10;
              if (((param_6 != 0) && (iVar7 = iVar9, iVar8 = iVar12, param_6 != 2)) &&
                 (iVar7 = iVar12, iVar8 = iVar9, param_6 != 4)) {
                iVar7 = iVar10;
                iVar8 = iVar11;
              }
              if ((iVar7 + param_1 == param_3) && (iVar8 + param_2 == param_4)) {
                bVar1 = true;
              }
              iVar11 = iVar11 + 1;
              iVar12 = iVar12 + -1;
            } while (iVar11 <= param_5);
          }
          param_5 = param_5 + 1;
          if (5 < param_5) {
            param_5 = 5;
          }
          iVar9 = iVar9 + 1;
          iVar10 = iVar10 + -1;
        } while (iVar9 <= DAT_00bc96e4 * 2);
      }
      if (((param_6 == param_7) && (iVar6 != 0)) && (bVar1)) {
        return 1;
      }
    }
  }
  return 0;
}
