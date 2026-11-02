#include "restore_types.h"
#define DAT_005cb9d8 (*(undefined4 *)0x005cb9d8u)
#define DAT_005cbcd8 (*(undefined4 *)0x005cbcd8u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9740 (*(undefined *)0x00bc9740u)
#define DAT_00bc9748 (*(undefined *)0x00bc9748u)
#define DAT_00bc9760 (*(undefined *)0x00bc9760u)
#define DAT_00bc9764 (*(undefined *)0x00bc9764u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f5686c (*(undefined *)0x00f5686cu)
extern void FUN_004746d0();
extern void FUN_0047db00();
extern undefined4 FUN_0047e000();
extern void FUN_0047e750();
extern int FUN_00484be0();
extern void FUN_00484f00();
extern void FUN_0051f780();

void FUN_0047afd0(int param_1,int param_2,int param_3)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  
  iVar3 = param_1 + -1;
  DAT_00bc9760 = param_2 + -1;
  param_1 = param_3;
  if (iVar3 < 0) {
    param_1 = iVar3 + param_3;
    iVar3 = 0;
  }
  else if (DAT_00bca2d0 < iVar3 + param_3) {
    param_1 = DAT_00bca2d0 - iVar3;
  }
  if (DAT_00bc9760 < 0) {
    param_3 = DAT_00bc9760 + param_3;
    DAT_00bc9760 = 0;
  }
  else if (DAT_00bca2d4 < DAT_00bc9760 + param_3) {
    param_3 = DAT_00bca2d4 - DAT_00bc9760;
  }
  param_3 = param_3 + DAT_00bc9760;
  DAT_00bc9768 = DAT_00bca2d8 + DAT_00bc9760 * 0xe4 + iVar3;
  if (DAT_00bc9760 < param_3) {
    iVar2 = iVar3;
    do {
      while (DAT_00bc9764 = iVar2, DAT_00bc9764 < param_1 + iVar3) {
        if ((((&DAT_00af598c)[DAT_00bc9768] & 0x4000) != 0) &&
           (((&DAT_00af598c)[DAT_00bc9768] & 8) == 0)) {
          FUN_0047db00(0x4000);
          FUN_00484f00();
          FUN_00484be0(&DAT_005cb9d8,0x30,0xffffffff,0);
          iVar2 = DAT_00bc9768;
          bVar1 = (&DAT_00b67d1c)[DAT_00bc9768];
          (&DAT_00b7482c)[DAT_00bc9768] = DAT_00f5686c + DAT_00bc9740 + DAT_00bc9748;
          (&DAT_00b67d1c)[iVar2] = bVar1 | 0x40;
          (&DAT_00b285cc)[iVar2] = (&DAT_00b285cc)[iVar2] & 0xf0;
          iVar2 = FUN_0047e000();
          if (iVar2 == 0) {
            uVar4 = 0;
LAB_0047b12d: ;
            FUN_0051f780(uVar4);
          }
          else {
            FUN_0047e750();
            iVar2 = FUN_00484be0(&DAT_005cbcd8,0xf,0xffffffff,0);
            if (iVar2 == 0) {
              uVar4 = 1;
              goto LAB_0047b12d;
            }
            (&DAT_00b7482c)[DAT_00bc9768] = DAT_00f5686c + DAT_00bc9740 + DAT_00bc9748;
          }
          FUN_004746d0(DAT_00bc9764,DAT_00bc9760,1);
        }
        DAT_00bc9768 = DAT_00bc9768 + 1;
        iVar2 = DAT_00bc9764 + 1;
      }
      DAT_00bc9760 = DAT_00bc9760 + 1;
      DAT_00bc9768 = DAT_00bc9768 + (0xe4 - param_1);
      iVar2 = iVar3;
    } while (DAT_00bc9760 < param_3);
  }
  return;
}
