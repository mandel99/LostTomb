#include "restore_types.h"
#define DAT_005df8b0 (*(undefined4 *)0x005df8b0u)
#define DAT_005df970 (*(undefined4 *)0x005df970u)
#define DAT_005df974 (*(undefined4 *)0x005df974u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a80 (*(undefined4 *)0x00799a80u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00bc9714 (*(undefined *)0x00bc9714u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5cfa (*(undefined4 *)0x00ea5cfau)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00ea6200 (*(undefined4 *)0x00ea6200u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern int FUN_0046a400();
extern int FUN_00475b50();
extern void FUN_0047bf50();
extern undefined4 FUN_00485100();
extern void FUN_004c0be0();
extern undefined4 FUN_004c5120();

void FUN_004c3420(int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
                 int param_6)

{
  short sVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  
  iVar7 = DAT_00eaac28;
  iVar8 = param_1 * 0x108;
  if (*(short *)(&DAT_00799a10 + iVar8) == 0) {
    iVar2 = param_1;
    if (param_6 == 0) {
      uVar6 = (uint)*(byte *)(&DAT_00799a82 + param_1 * 0x84);
      iVar7 = 0;
    }
    else {
      *(undefined1 *)((int)&DAT_00799a80 + iVar8 + 1) = 0;
      (&DAT_00ea5ce8)[iVar7 * 0x24ec] = 1;
      (&DAT_00ea5cfa)[iVar7 * 0x24ec] = 2;
      (&DAT_00ea5f9c)[iVar7 * 0x1276] = param_1;
      uVar6 = DAT_00bc9714 * 2 - DAT_00bca2e0;
      if ((int)uVar6 < 0) {
        uVar6 = uVar6 + 8;
      }
      *(char *)(&DAT_00799a82 + param_1 * 0x84) = (char)uVar6;
      iVar7 = 0;
    }
    do {
      if (iVar7 == 0) {
        iVar3 = *(int *)(&DAT_005df8b0 + (DAT_00bca2e0 / 2 + ((int)uVar6 / 2) * 4) * 0xc);
        if (param_6 != 0) {
          *(undefined2 *)(&DAT_00799a10 + iVar8) = 0;
          goto LAB_004c361e;
        }
      }
      else {
        if (iVar7 == 1) {
          if (param_6 != 0) {
            iVar2 = FUN_0046a400(param_4,param_5,*(int *)(&DAT_005df970 + uVar6 * 8) + param_2,
                                 *(int *)(&DAT_005df974 + uVar6 * 8) + param_3,0,0);
            iVar8 = iVar2 * 0x108;
            *(char *)(&DAT_00799a82 + iVar2 * 0x84) = (char)uVar6;
            FUN_00485100(iVar2);
            *(short *)(&DAT_00799a10 + iVar8) = (short)param_1;
            *(short *)(&DAT_00799a12 + param_1 * 0x108) = (short)iVar2;
            *(undefined2 *)(&DAT_00799a12 + iVar8) = 0;
          }
          iVar3 = 0x23a;
        }
        else {
          sVar1 = *(short *)(&DAT_00799a12 + param_1 * 0x108);
          if (param_6 != 0) {
            iVar2 = FUN_0046a400(param_4,param_5,
                                 *(int *)(&DAT_005df970 + uVar6 * 8) * iVar7 + param_2,
                                 *(int *)(&DAT_005df974 + uVar6 * 8) * iVar7 + param_3,0,0);
            iVar8 = iVar2 * 0x108;
            *(char *)(&DAT_00799a82 + iVar2 * 0x84) = (char)uVar6;
            FUN_00485100(iVar2);
            *(short *)(&DAT_00799a10 + iVar8) = sVar1;
            *(short *)(&DAT_00799a12 + sVar1 * 0x108) = (short)iVar2;
            *(undefined2 *)(&DAT_00799a12 + iVar8) = 0;
          }
          iVar3 = *(int *)(&DAT_005df8b0 +
                          (iVar7 + (DAT_00bca2e0 / 2 + ((int)uVar6 / 2) * 4) * 3) * 4);
        }
LAB_004c361e: ;
        if (param_6 != 0) {
          switch((int)(uVar6)) {
          case 0: ;
          case 4: ;
            *(short *)((int)&DAT_007999ea + iVar8) = (short)iVar7;
            break;
          case 2: ;
          case 6: ;
            *(short *)((int)&DAT_007999ea + iVar8) = (short)iVar7 + 3;
          }
        }
      }
      iVar5 = 0;
      if ((DAT_00bca2e0 == 0) || (DAT_00bca2e0 == 4)) {
        if ((uVar6 == 2) || (uVar6 == 6)) goto LAB_004c366c;
      }
      else if ((uVar6 != 2) && (uVar6 != 6)) {
LAB_004c366c: ;
        iVar5 = 3;
      }
      FUN_0047bf50(iVar2,*(int *)(&DAT_005df970 + uVar6 * 8) * iVar7 + param_2,
                   *(int *)(&DAT_005df974 + uVar6 * 8) * iVar7 + param_3,3,3,
                   (&DAT_00f55bb4)[iVar3 * 2] + iVar5,8);
      iVar7 = iVar7 + 1;
    } while (iVar7 < 3);
    uVar4 = FUN_00475b50(iVar2);
    FUN_004c0be0(uVar4);
    iVar7 = FUN_004c5120(iVar2);
    if (iVar7 == 4) {
      *(undefined4 *)(&DAT_00ea6200 + DAT_00eaac28 * 0x49d8) = 10;
      return;
    }
    *(undefined4 *)(&DAT_00ea6200 + DAT_00eaac28 * 0x49d8) = 0;
  }
  return;
}
