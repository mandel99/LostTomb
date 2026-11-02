#include "restore_types.h"
#define DAT_009d1b1c (*(undefined4 *)0x009d1b1cu)
#define DAT_009de62c (*(undefined4 *)0x009de62cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00d05720 (*(undefined4 *)0x00d05720u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
extern void FUN_0040f860();
extern undefined4 FUN_0046f930();
extern uint FUN_00470900();
extern void FUN_0047ad20();
extern void FUN_00485830();
extern void FUN_00516f90();
extern void FUN_005173a0();
extern void FUN_005178e0();

void FUN_00475220(int param_1,int param_2,int param_3,short *param_4,int param_5,undefined4 param_6)

{
  char cVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  FUN_0040f860(&DAT_009de62c,&DAT_00af598c,0x32c40);
  FUN_0040f860(&DAT_009d1b1c,&DAT_00ae8e7c,0xcb10);
  FUN_00485830(0);
  iVar2 = DAT_00bca2d0 + -1;
  iVar7 = param_2;
  if (iVar2 <= param_2) {
    iVar7 = iVar2;
  }
  if (iVar7 < 0) {
    param_2 = 0;
    iVar7 = param_2;
LAB_0047527c: ;
    param_2 = iVar7;
  }
  else {
    iVar7 = iVar2;
    if (iVar2 <= param_2) goto LAB_0047527c;
  }
  iVar7 = DAT_00bca2d4 + -1;
  iVar6 = param_3;
  if (iVar7 <= param_3) {
    iVar6 = iVar7;
  }
  if (iVar6 < 0) {
    param_3 = 0;
    iVar6 = param_3;
LAB_004752a7: ;
    param_3 = iVar6;
  }
  else {
    iVar6 = iVar7;
    if (iVar7 <= param_3) goto LAB_004752a7;
  }
  iVar6 = (int)param_4;
  if (iVar2 <= (int)param_4) {
    iVar6 = iVar2;
  }
  if (iVar6 < 0) {
    iVar6 = 0;
  }
  else {
    iVar6 = (int)param_4;
    if (iVar2 <= (int)param_4) {
      iVar6 = iVar2;
    }
  }
  iVar2 = param_5;
  if (iVar7 <= param_5) {
    iVar2 = iVar7;
  }
  if (iVar2 < 0) {
    param_5 = 0;
    iVar7 = param_5;
LAB_004752e3: ;
    param_5 = iVar7;
  }
  else if (iVar7 <= param_5) goto LAB_004752e3;
  uVar4 = iVar6 - param_2 >> 0x1f;
  uVar5 = param_5 - param_3 >> 0x1f;
  if ((int)((param_5 - param_3 ^ uVar5) - uVar5) < (int)((iVar6 - param_2 ^ uVar4) - uVar4)) {
    if (param_3 < param_5) {
      if (param_3 + 1 <= param_5) {
        param_5 = param_3 + 1;
      }
    }
    else if ((param_5 < param_3) && (param_5 <= param_3 + -1)) {
      param_5 = param_3 + -1;
    }
  }
  else if (param_2 < iVar6) {
    iVar7 = param_2 + 1;
    if (iVar7 <= iVar6) {
LAB_00475339: ;
      iVar6 = iVar7;
    }
  }
  else {
    if (param_2 <= iVar6) goto LAB_0047534f;
    iVar7 = param_2 + -1;
    if (iVar6 <= iVar7) goto LAB_00475339;
  }
  if (iVar6 < param_2) {
    iVar6 = iVar6 + param_2;
    param_2 = iVar6 - param_2;
    iVar6 = iVar6 - param_2;
  }
LAB_0047534f: ;
  if (param_5 < param_3) {
    param_5 = param_5 + param_3;
    param_3 = param_5 - param_3;
    param_5 = param_5 - param_3;
  }
  DAT_00bc971c = 0;
  iVar3 = DAT_00bca2d0 - iVar6;
  iVar7 = DAT_00bca2d8 + param_3 * 0xe4 + param_2;
  for (iVar2 = param_3; iVar2 <= param_5; iVar2 = iVar2 + 1) {
    if (param_2 <= iVar6) {
      param_4 = (short *)(&DAT_00b350dc + iVar7 * 2);
      iVar8 = param_2;
      do {
        if (((((&DAT_00af598c)[iVar7] & 0x2635177f) == 0) &&
            (((*(int *)(&DAT_00d05720 + iVar7 * 4) == 0 ||
              (*(char *)(*(int *)(&DAT_00d05720 + iVar7 * 4) + 0x18) != '\0')) &&
             (cVar1 = FUN_0046f930(DAT_00bca2e8 + iVar8,DAT_00bca2ec + iVar2), cVar1 != '\0')))) &&
           ((cVar1 = FUN_00470900(iVar8,iVar2,iVar8,iVar2), cVar1 == '\0' && (*param_4 == 0)))) {
          DAT_00bc9768 = iVar7;
          FUN_0047ad20(iVar8,iVar2,1,param_6);
        }
        iVar8 = iVar8 + 1;
        iVar7 = iVar7 + 1;
        param_4 = param_4 + 1;
      } while (iVar8 <= iVar6);
    }
    iVar7 = iVar7 + DAT_00bca2dc + iVar3 + -1 + param_2;
  }
  if (param_1 == 0) {
    FUN_00516f90(param_2,param_3,iVar6,param_5,1);
    FUN_005173a0(param_2,param_3,iVar6,param_5);
    FUN_005178e0();
    DAT_00e38dca = 1;
  }
  return;
}
