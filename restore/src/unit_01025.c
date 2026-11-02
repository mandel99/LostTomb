#include "restore_types.h"
#define LAB_004021d0 (*(undefined1 *)0x004021d0u)
#define LAB_00402210 (*(undefined1 *)0x00402210u)
#define LAB_00402250 (*(undefined1 *)0x00402250u)
#define LAB_00402290 (*(undefined1 *)0x00402290u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9698 (*(undefined *)0x00bc9698u)
#define DAT_00bc969c (*(undefined *)0x00bc969cu)
#define DAT_00bc96a0 (*(undefined *)0x00bc96a0u)
#define DAT_00bc96a4 (*(undefined *)0x00bc96a4u)
#define DAT_00bc96a8 (*(undefined *)0x00bc96a8u)
#define DAT_00bc96ac (*(undefined *)0x00bc96acu)
#define DAT_00bc96b0 (*(undefined *)0x00bc96b0u)
#define DAT_00bc96b4 (*(undefined *)0x00bc96b4u)
#define DAT_00bc9760 (*(undefined *)0x00bc9760u)
#define DAT_00bc9764 (*(undefined *)0x00bc9764u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern void FUN_004264d0();
extern undefined4 FUN_00474380();
extern void FUN_00474700();

void FUN_004743f0(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  short sVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  DAT_00bc96b4 = 0;
  DAT_00bc96b0 = 0;
  DAT_00bc96ac = 0;
  DAT_00bc96a8 = 0;
  if (param_1 != 0) {
    FUN_00474700(param_1,param_2,param_3,param_4,param_5);
    return;
  }
  iVar8 = param_4;
  iVar9 = param_2;
  if (param_2 <= param_4) {
    iVar8 = param_2;
    iVar9 = param_4;
  }
  if (param_5 < param_3) {
    param_1 = param_3;
    DAT_00bc9760 = param_5;
  }
  else {
    param_1 = param_5;
    DAT_00bc9760 = param_3;
  }
  if (iVar8 < 0) {
    iVar8 = 0;
  }
  if (DAT_00bca2d0 <= iVar9) {
    iVar9 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bc9760 < 0) {
    DAT_00bc9760 = 0;
  }
  if (DAT_00bca2d4 <= param_1) {
    param_1 = DAT_00bca2d4 + -1;
  }
  bVar4 = false;
  bVar5 = false;
  bVar2 = false;
  bVar3 = false;
  DAT_00bc9768 = DAT_00bca2d8 + DAT_00bc9760 * 0xe4 + iVar8;
  for (; iVar7 = iVar8, DAT_00bc9760 <= param_1; DAT_00bc9760 = DAT_00bc9760 + 1) {
    while (DAT_00bc9764 = iVar7, DAT_00bc9764 <= iVar9) {
      iVar7 = (int)*(short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2);
      if ((iVar7 != 0) &&
         (((&DAT_007999e8)[iVar7 * 0x84] == 0x39 || ((&DAT_007999e8)[iVar7 * 0x84] == 0x36)))) {
        bVar4 = true;
      }
      if ((((&DAT_00adc36c)[DAT_00bc9768] != '\0') && (((&DAT_00af598c)[DAT_00bc9768] & 4) != 0)) &&
         (iVar7 == 0)) {
        bVar5 = true;
      }
      cVar6 = FUN_00474380((int)(short)(&DAT_007999e8)[iVar7 * 0x84]);
      if (cVar6 != '\0') {
        bVar2 = true;
      }
      sVar1 = (&DAT_007999e8)[iVar7 * 0x84];
      if (((((sVar1 == 0x41) || (sVar1 == 0x42)) ||
           ((sVar1 == 0x43 || ((sVar1 == 0x44 || (sVar1 == 0x45)))))) || (sVar1 == 0x61)) ||
         ((((sVar1 == 0x9c || (sVar1 == 0x9d)) || (sVar1 == 0x9e)) ||
          ((sVar1 == 0x9f || (sVar1 == 0xa0)))))) {
        bVar3 = true;
      }
      DAT_00bc9768 = DAT_00bc9768 + 1;
      iVar7 = DAT_00bc9764 + 1;
    }
    DAT_00bc9768 = DAT_00bc9768 + (0xe4 - ((iVar9 - iVar8) + 1));
  }
  DAT_00bc96a4 = param_2;
  DAT_00bc969c = param_4;
  DAT_00bc9698 = param_5;
  DAT_00bc96a0 = param_3;
  if (bVar2) {
    FUN_004264d0(0x68,&LAB_004021d0,2);
    return;
  }
  if (bVar3) {
    FUN_004264d0(0x6a,&LAB_00402210,2);
    return;
  }
  if (bVar4) {
    FUN_004264d0(0x10,&LAB_00402250,2);
    return;
  }
  if (bVar5) {
    FUN_004264d0(0x14,&LAB_00402290,2);
    return;
  }
  FUN_00474700(0,param_2,param_3,param_4,param_5);
  return;
}
