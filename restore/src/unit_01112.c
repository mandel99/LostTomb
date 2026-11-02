#include "restore_types.h"
#define DAT_005cc678 (*(undefined4 *)0x005cc678u)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9740 (*(undefined *)0x00bc9740u)
#define DAT_00bc9744 (*(undefined1 *)0x00bc9744u)
#define DAT_00bc9748 (*(undefined *)0x00bc9748u)
#define DAT_00bc9760 (*(undefined *)0x00bc9760u)
#define DAT_00bc9764 (*(undefined *)0x00bc9764u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f56b84 (*(undefined *)0x00f56b84u)
#define _DAT_00bca328 (*(undefined4 *)0x00bca328u)
extern uint FUN_0047b920();
extern void FUN_0047e870();
extern int FUN_00484be0();
extern int FUN_0051c620();

void FUN_0047bae0(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  byte bVar1;
  undefined1 uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  bool bVar6;
  
  iVar4 = param_1;
  if (param_1 <= param_3) {
    iVar4 = param_3;
    param_3 = param_1;
  }
  if (param_4 < param_2) {
    param_1 = param_2;
    param_2 = param_4;
  }
  else {
    param_1 = param_4;
  }
  param_3 = param_3 + -1;
  iVar4 = iVar4 + 1;
  DAT_00bc9760 = param_2 + -1;
  param_1 = param_1 + 1;
  if (param_3 < 0) {
    param_3 = 0;
  }
  if (DAT_00bca2d0 <= iVar4) {
    iVar4 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bc9760 < 0) {
    DAT_00bc9760 = 0;
  }
  if (DAT_00bca2d4 <= param_1) {
    param_1 = DAT_00bca2d4 + -1;
  }
  DAT_00bc9768 = DAT_00bca2d8 + DAT_00bc9760 * 0xe4 + param_3;
  iVar3 = param_3;
  if (DAT_00bc9760 <= param_1) {
    do {
      while (DAT_00bc9764 = iVar3, DAT_00bc9764 <= iVar4) {
        if (((((&DAT_00af598c)[DAT_00bc9768] & 0x100) != 0) &&
            (((&DAT_00af598c)[DAT_00bc9768] & 0xc) == 0)) &&
           ((byte)(&DAT_00ae8e7c)[DAT_00bc9768] < 0x10)) {
          if ((&DAT_00b7482c)[DAT_00bc9768] - DAT_00f56b84 < 0x30) {
            uVar5 = (-1 < (&DAT_00b7482c)[DAT_00bc9768] - DAT_00f56b84) - 1 & 0x30;
          }
          else {
            uVar5 = 0x30;
          }
          if (param_5 != 0) {
            uVar5 = 0x30;
          }
          FUN_0047e870();
          FUN_00484be0(&DAT_005cc678,0x10,0xffffffff,0);
          iVar3 = DAT_00bc9768;
          if ((*(byte *)(&DAT_00af598c + DAT_00bc9768) & 0x40) != 0) {
            bVar6 = DAT_00bc9744 == 0;
            (&DAT_00b285cc)[DAT_00bc9768] = (&DAT_00b285cc)[DAT_00bc9768] & 0x7f;
            if (bVar6) {
              DAT_00bc9748 = (byte)~(byte)*(undefined4 *)(&DAT_00af55fc + iVar3 * 4) >> 6 & 1;
            }
            iVar3 = FUN_0047b920();
            if (iVar3 == 0) {
              DAT_00bc9748 = DAT_00bc9748 + 0xf;
            }
            else {
              DAT_00bc9748 = DAT_00bc9748 + 0x2a;
            }
          }
          iVar3 = DAT_00f56b84 + DAT_00bc9740 + DAT_00bc9748 + uVar5;
          if (((&DAT_00af598c)[DAT_00bc9768] & 0x10000) != 0) {
            iVar3 = FUN_0051c620(iVar3);
          }
          bVar1 = (&DAT_00b285cc)[DAT_00bc9768];
          (&DAT_00b7482c)[DAT_00bc9768] = iVar3;
          (&DAT_00b285cc)[DAT_00bc9768] = bVar1 & 0xf0;
          uVar2 = (undefined1)DAT_00bc9744;
          (&DAT_00b67d1c)[DAT_00bc9768] = (&DAT_00b67d1c)[DAT_00bc9768] | 0x40;
          (&DAT_00ae8e7c)[DAT_00bc9768] = uVar2;
        }
        DAT_00bc9768 = DAT_00bc9768 + 1;
        iVar3 = DAT_00bc9764 + 1;
      }
      DAT_00bc9760 = DAT_00bc9760 + 1;
      DAT_00bc9768 = DAT_00bc9768 + (0xe4 - ((iVar4 - param_3) + 1));
      iVar3 = param_3;
    } while (DAT_00bc9760 <= param_1);
    _DAT_00bca328 = 0;
    return;
  }
  _DAT_00bca328 = 0;
  return;
}
