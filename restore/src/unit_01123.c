#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)

void FUN_0047ca50(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  int iVar2;
  
  if ((((-1 < param_2) && (-1 < param_3)) && (param_2 + -1 + param_4 < DAT_00bca2d0)) &&
     (param_3 + -1 + param_5 < DAT_00bca2d4)) {
    param_2 = DAT_00bca2d8 + param_3 * 0xe4 + param_2;
    DAT_00bc9768 = param_2;
    if (0 < param_5) {
      iVar2 = 0;
      do {
        iVar1 = 0;
        if (0 < param_4) {
          do {
            if (param_1 == 0) {
              (&DAT_00eed760)[(&DAT_005cc8c0)[(iVar2 + iVar1) * 2] + param_2] = 0;
            }
            iVar1 = iVar1 + 1;
          } while (iVar1 < param_4);
        }
        iVar2 = iVar2 + 6;
        param_5 = param_5 + -1;
      } while (param_5 != 0);
    }
  }
  return;
}
