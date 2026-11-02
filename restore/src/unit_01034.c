#include "restore_types.h"
#define DAT_009ab9ec (*(undefined4 *)0x009ab9ecu)
#define DAT_009b84fc (*(undefined4 *)0x009b84fcu)
#define DAT_009d1b1c (*(undefined4 *)0x009d1b1cu)
#define DAT_009de62c (*(undefined4 *)0x009de62cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00d05720 (*(undefined4 *)0x00d05720u)
extern void FUN_0040f860();
extern uint FUN_00470900();
extern void FUN_00476530();
extern void FUN_00485830();

void FUN_004759e0(undefined4 param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = param_5;
  iVar4 = param_4;
  if (param_4 < param_2) {
    param_4 = param_2;
    param_2 = iVar4;
  }
  if (param_5 < param_3) {
    param_5 = param_3;
    param_3 = iVar3;
  }
  FUN_0040f860(&DAT_009de62c,&DAT_00af598c,0x32c40);
  FUN_0040f860(&DAT_009d1b1c,&DAT_00ae8e7c,0xcb10);
  FUN_0040f860(&DAT_009b84fc,&DAT_00b285cc,0xcb10);
  FUN_0040f860(&DAT_009ab9ec,&DAT_00b67d1c,0xcb10);
  FUN_00485830(0);
  if (param_2 < 0) {
    param_2 = 0;
  }
  if (param_3 < 0) {
    param_3 = 0;
  }
  if (DAT_00bca2d0 <= param_4) {
    param_4 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= param_5) {
    param_5 = DAT_00bca2d4 + -1;
  }
  iVar3 = DAT_00bca2d0 - param_4;
  DAT_00bc971c = 0;
  iVar4 = DAT_00bca2d8 + param_3 * 0xe4 + param_2;
  for (; iVar1 = param_2, param_3 <= param_5; param_3 = param_3 + 1) {
    for (; iVar1 <= param_4; iVar1 = iVar1 + 1) {
      if ((((&DAT_00af598c)[iVar4] & 0xeeffd77f) == 0) &&
         (((*(int *)(&DAT_00d05720 + iVar4 * 4) == 0 ||
           (*(char *)(*(int *)(&DAT_00d05720 + iVar4 * 4) + 0x18) != '\0')) &&
          (cVar2 = FUN_00470900(iVar1,param_3,iVar1,param_3), cVar2 == '\0')))) {
        DAT_00bc971c = DAT_00bc971c + 1;
        (&DAT_00af598c)[iVar4] = (&DAT_00af598c)[iVar4] | 0x20;
      }
      iVar4 = iVar4 + 1;
    }
    iVar4 = iVar4 + DAT_00bca2dc + iVar3 + -1 + param_2;
  }
  FUN_00476530();
  return;
}
