#include "restore_types.h"
#define DAT_005cc678 (*(undefined4 *)0x005cc678u)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9740 (*(undefined *)0x00bc9740u)
#define DAT_00bc9744 (*(undefined1 *)0x00bc9744u)
#define DAT_00bc9748 (*(undefined *)0x00bc9748u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00f56b84 (*(undefined *)0x00f56b84u)
extern uint FUN_0047b920();
extern void FUN_0047e870();
extern int FUN_00484be0();
extern int FUN_0051c620();

void FUN_0047be30(int param_1)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  bool bVar4;
  
  if (*(short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2) == 0) {
    if (((&DAT_00b7482c)[DAT_00bc9768] - DAT_00f56b84 < 0) &&
       (0x2f < (&DAT_00b7482c)[DAT_00bc9768] - DAT_00f56b84)) {
      iVar3 = 0x30;
    }
    else {
      iVar3 = 0;
    }
    if (param_1 != 0) {
      iVar3 = 0x30;
    }
    FUN_0047e870();
    FUN_00484be0(&DAT_005cc678,0x10,0xffffffff,0);
    iVar2 = DAT_00bc9768;
    if ((*(byte *)(&DAT_00af598c + DAT_00bc9768) & 0x40) != 0) {
      bVar4 = DAT_00bc9744 == 0;
      (&DAT_00b285cc)[DAT_00bc9768] = (&DAT_00b285cc)[DAT_00bc9768] & 0x7f;
      if (bVar4) {
        DAT_00bc9748 = (uint)((*(uint *)(&DAT_00af55fc + iVar2 * 4) & 0x8000040) == 0);
      }
      iVar2 = FUN_0047b920();
      if (iVar2 == 0) {
        DAT_00bc9748 = DAT_00bc9748 + 0xf;
      }
      else {
        DAT_00bc9748 = DAT_00bc9748 + 0x2a;
      }
    }
    iVar3 = DAT_00bc9740 + DAT_00f56b84 + DAT_00bc9748 + iVar3;
    if (((&DAT_00af598c)[DAT_00bc9768] & 0x10000) != 0) {
      iVar3 = FUN_0051c620(iVar3);
    }
    iVar2 = DAT_00bc9768;
    bVar1 = (&DAT_00b285cc)[DAT_00bc9768];
    (&DAT_00b7482c)[DAT_00bc9768] = iVar3;
    (&DAT_00b285cc)[iVar2] = bVar1 & 0xf0;
    (&DAT_00ae8e7c)[iVar2] = (undefined1)DAT_00bc9744;
    (&DAT_00b67d1c)[iVar2] = (&DAT_00b67d1c)[iVar2] | 0x40;
  }
  return;
}
