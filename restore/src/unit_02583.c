#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
extern void FUN_005468d0();

void FUN_00547a60(void)

{
  short sVar1;
  int iVar2;
  short *psVar3;
  
  iVar2 = 1;
  if (0 < DAT_0078d790) {
    psVar3 = &DAT_00799af0;
    do {
      sVar1 = *psVar3;
      if ((((sVar1 == 0xe5) || (sVar1 == 0xea)) || (sVar1 == 0xeb)) || (sVar1 == 0xec)) {
        FUN_005468d0(iVar2);
      }
      iVar2 = iVar2 + 1;
      psVar3 = psVar3 + 0x84;
    } while (iVar2 <= DAT_0078d790);
  }
  return;
}
