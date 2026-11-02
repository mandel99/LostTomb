#include "restore_types.h"
#define DAT_006fdd18 (*(undefined4 *)0x006fdd18u)
#define DAT_006ff608 (*(undefined *)0x006ff608u)
#define DAT_00e38eab (*(char *)0x00e38eabu)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)

undefined4 FUN_004181a0(void)

{
  int iVar1;
  
  if ((((DAT_00e38ebc == 0) && (iVar1 = (int)DAT_00e38eab, -1 < iVar1)) && (iVar1 < DAT_006ff608))
     && (iVar1 * 0x40 != -0x6fdd08)) {
    return *(undefined4 *)(&DAT_006fdd18 + iVar1 * 0x40);
  }
  return 0x25;
}
