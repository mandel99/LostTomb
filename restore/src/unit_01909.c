#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
extern void FUN_004dfbb0();

void FUN_004e0c80(int param_1)

{
  short sVar1;
  int iVar2;
  
  if ((&DAT_007999e8)[param_1 * 0x84] == 0xde) {
    sVar1 = (&DAT_007999ea)[param_1 * 0x84];
    iVar2 = 0;
    do {
      FUN_004dfbb0(iVar2 + (param_1 - sVar1));
      iVar2 = iVar2 + 1;
    } while (iVar2 < 0x74);
  }
  return;
}
