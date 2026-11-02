#include "restore_types.h"
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00f56c4c (*(undefined *)0x00f56c4cu)
extern undefined4 FUN_0046f930();

void FUN_0047a120(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  char cVar4;
  int iVar5;
  
  cVar4 = FUN_0046f930(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2);
  iVar3 = DAT_00f56c4c;
  if (cVar4 != '\0') {
    iVar1 = DAT_00bca2d8 + param_2 * 0xe4;
    iVar5 = iVar1 + param_1;
    DAT_00bc9768 = iVar5;
    (&DAT_00b285cc)[iVar5] = (&DAT_00b285cc)[param_1 + iVar1] & 0xf0;
    uVar2 = (&DAT_00af598c)[iVar5];
    (&DAT_00ae8e7c)[iVar5] = 0;
    (&DAT_00b7482c)[iVar5] = ((byte)(&DAT_00acf85c)[iVar5] & 7) + iVar3;
    (&DAT_00b67d1c)[iVar5] = (&DAT_00b67d1c)[iVar5] | 0x40;
    (&DAT_00af598c)[iVar5] = uVar2 | 0x1000;
  }
  return;
}
