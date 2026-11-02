#include "restore_types.h"
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_004c5120();
extern bool FUN_004c52d0();

undefined4 FUN_004c5340(int param_1)

{
  char cVar1;
  int iVar2;
  
  if (((&DAT_00ea5ce8)[param_1 * 0x24ec] != 0) &&
     (iVar2 = FUN_004c5120((&DAT_00ea5f9c)[param_1 * 0x1276]), iVar2 == 4)) {
    return 0x14;
  }
  if ((((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) &&
      (iVar2 = FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]), iVar2 == 0)) &&
     (cVar1 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],1), cVar1 != '\0')) {
    return 0x14;
  }
  return 0x19;
}
