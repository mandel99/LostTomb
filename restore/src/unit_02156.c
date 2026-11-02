#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00ea5fa0 (*(undefined4 *)0x00ea5fa0u)
#define DAT_00ea5fac (*(undefined4 *)0x00ea5facu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004e5950();
extern void FUN_004f4a80();

void FUN_004f4ae0(int param_1)

{
  int *piVar1;
  int iVar2;
  
  if (*(short *)(&DAT_00799a10 + param_1 * 0x108) == 0) {
    iVar2 = 0;
    piVar1 = (int *)(&DAT_00ea5fac + DAT_00eaac28 * 0x49d8);
    while (*piVar1 != param_1) {
      iVar2 = iVar2 + 1;
      piVar1 = piVar1 + 1;
      if (2 < iVar2) {
        return;
      }
    }
    (&DAT_00ea5fa0)[DAT_00eaac28 * 0x1276 + iVar2] = 0;
    if (iVar2 < 3) {
      if (((&DAT_007999e8)[param_1 * 0x84] == 0xcf) || ((&DAT_007999e8)[param_1 * 0x84] == 0xd0)) {
        FUN_004e5950(iVar2);
      }
      FUN_004f4a80(param_1,iVar2);
    }
  }
  return;
}
