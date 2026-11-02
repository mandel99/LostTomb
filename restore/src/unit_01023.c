#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bc9760 (*(undefined *)0x00bc9760u)
#define DAT_00bc9764 (*(undefined *)0x00bc9764u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00d05720 (*(undefined4 *)0x00d05720u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f5653c (*(undefined *)0x00f5653cu)
extern int FUN_0046a400();
extern void FUN_0047bf50();
extern undefined4 FUN_00482650();
extern void FUN_00485080();
extern undefined4 FUN_00485100();
extern void FUN_004e2970();
extern void FUN_004e2d80();
extern void FUN_004e2dd0();
extern void FUN_004e2fb0();
extern void FUN_004e3330();
extern void FUN_00516f90();
extern void FUN_005173a0();

void FUN_00474130(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int local_8;
  
  bVar1 = false;
  iVar2 = param_2;
  iVar3 = param_4;
  if (param_2 <= param_4) {
    iVar2 = param_4;
    iVar3 = param_2;
  }
  if (param_5 < param_3) {
    local_8 = param_3;
    iVar4 = param_5;
  }
  else {
    local_8 = param_5;
    iVar4 = param_3;
  }
  if (iVar3 < 0) {
    iVar3 = 0;
  }
  if (DAT_00bca2d0 <= iVar2) {
    iVar2 = DAT_00bca2d0 + -1;
  }
  if (iVar4 < 0) {
    iVar4 = 0;
  }
  if (DAT_00bca2d4 <= local_8) {
    local_8 = DAT_00bca2d4 + -1;
  }
  DAT_00bc971c = 0;
  FUN_00485080();
  DAT_00bc9768 = DAT_00bca2d8 + iVar4 * 0xe4 + iVar3;
  for (DAT_00bc9760 = iVar4; iVar4 = iVar3, DAT_00bc9760 <= local_8; DAT_00bc9760 = DAT_00bc9760 + 1
      ) {
    while (DAT_00bc9764 = iVar4, DAT_00bc9764 <= iVar2) {
      if (((((&DAT_00af598c)[DAT_00bc9768] & 0x10000) == 0) &&
          (((*(int *)(&DAT_00d05720 + DAT_00bc9768 * 4) == 0 ||
            (*(char *)(*(int *)(&DAT_00d05720 + DAT_00bc9768 * 4) + 0x18) != '\0')) &&
           (((&DAT_00af598c)[DAT_00bc9768] & 0xeefed77f) == 0)))) &&
         (*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2) == 0)) {
        if (param_1 == 0) {
          iVar4 = FUN_0046a400(DAT_00eaac28,10,DAT_00bc9764,DAT_00bc9760,0,0);
          FUN_00485100(iVar4);
          if (0 < iVar4) {
            DAT_00bc971c = DAT_00bc971c + 1;
            FUN_0047bf50(iVar4,DAT_00bc9764,DAT_00bc9760,1,1,DAT_00f5653c,8);
            iVar4 = FUN_00482650(DAT_00bc9764,DAT_00bc9760,1,2,0x40);
            if (iVar4 == 0) {
              bVar1 = true;
            }
          }
        }
        else {
          DAT_00bc971c = DAT_00bc971c + 1;
          (&DAT_00b285cc)[DAT_00bc9768] = (&DAT_00b285cc)[DAT_00bc9768] | 0x10;
        }
      }
      DAT_00bc9768 = DAT_00bc9768 + 1;
      iVar4 = DAT_00bc9764 + 1;
    }
    DAT_00bc9768 = DAT_00bc9768 + (0xe4 - ((iVar2 - iVar3) + 1));
  }
  if (param_1 == 0) {
    FUN_004e3330(10);
    FUN_004e2d80(10);
    FUN_004e2dd0(10);
    FUN_004e2fb0(10);
    if (bVar1) {
      FUN_004e2970(0x3e,0,0);
    }
    FUN_00516f90(param_2,param_3,param_4,param_5,1);
    FUN_005173a0(param_2,param_3,param_4,param_5);
    DAT_00e38dca = 1;
  }
  return;
}
