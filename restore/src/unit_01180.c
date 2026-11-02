#include "restore_types.h"
#define DAT_00705c1c (*(undefined *)0x00705c1cu)
#define DAT_00ea5cd0 (*(undefined4 *)0x00ea5cd0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

int FUN_00480860(int param_1)

{
  int iVar1;
  short *psVar2;
  
  if (0 < DAT_00705c1c) {
    iVar1 = 0;
    psVar2 = (short *)(&DAT_00ea5cd0 + DAT_00eaac28 * 0x1276);
    do {
      if (param_1 == *psVar2) {
        return iVar1;
      }
      iVar1 = iVar1 + 1;
      psVar2 = psVar2 + 1;
    } while (iVar1 < 10);
  }
  return -1;
}
