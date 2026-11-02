#include "restore_types.h"
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern undefined4 FUN_0046f930();

void FUN_004c0860(int param_1,int param_2,int param_3,int param_4)

{
  byte bVar1;
  int iVar2;
  char cVar3;
  
  cVar3 = FUN_0046f930(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2);
  if (cVar3 != '\0') {
    param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    bVar1 = (&DAT_00b67d1c)[param_1];
    DAT_00bc9768 = param_1;
    (&DAT_00b7482c)[param_1] = (&DAT_00f55bb4)[param_3 * 2] + param_4;
    (&DAT_00b67d1c)[param_1] = bVar1 | 0x40;
    iVar2 = DAT_00eaac28;
    (&DAT_00ae8e7c)[param_1] = 0;
    (&DAT_00b285cc)[param_1] = (&DAT_00b285cc)[param_1] & 0xf0;
    *(undefined2 *)(&DAT_00b4e6fc + param_1 * 2) = *(undefined2 *)(&DAT_00ea5f9c + iVar2 * 0x1276);
    (&DAT_00af598c)[param_1] = (&DAT_00af598c)[param_1] | 8;
  }
  return;
}
