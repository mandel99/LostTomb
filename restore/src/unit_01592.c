#include "restore_types.h"
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_004c5120();
extern bool FUN_004c52d0();

undefined4 FUN_004c38f0(void)

{
  char cVar1;
  int iVar2;
  
  if ((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) {
    iVar2 = FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]);
    if (iVar2 == 1) {
      cVar1 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],2);
      if (cVar1 != '\0') {
        return 0xfffffffe;
      }
    }
  }
  return 0;
}
