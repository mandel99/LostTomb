#include "restore_types.h"
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00f56ba4 (*(undefined *)0x00f56ba4u)
extern undefined4 FUN_0046f930();

void FUN_00479ac0(int param_1,int param_2,int param_3)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  
  cVar2 = FUN_0046f930(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2);
  if (cVar2 != '\0') {
    param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    DAT_00bc9768 = param_1;
    if (param_3 != 1) {
      (&DAT_00af598c)[param_1] = (&DAT_00af598c)[param_1] & 0x2000000;
      return;
    }
    iVar3 = ((byte)(&DAT_00acf85c)[param_1] & 7) + DAT_00f56ba4;
    (&DAT_00af598c)[param_1] = (&DAT_00af598c)[param_1] | 0x2000000;
    bVar1 = (&DAT_00b67d1c)[param_1];
    (&DAT_00b7482c)[param_1] = iVar3;
    (&DAT_00b285cc)[param_1] = (&DAT_00b285cc)[param_1] & 0xf0;
    (&DAT_00b67d1c)[param_1] = bVar1 | 0x40;
  }
  return;
}
