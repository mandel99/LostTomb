#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

undefined4 FUN_0047cae0(int param_1,int *param_2,int param_3,int param_4,uint param_5,int param_6)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = param_1;
  if (param_1 < 0) {
    return 0;
  }
  if ((int)param_2 < 0) {
    return 0;
  }
  if (DAT_00bca2d0 <= param_1 + -1 + param_3) {
    return 0;
  }
  if (DAT_00bca2d4 <= (int)param_2 + -1 + param_4) {
    return 0;
  }
  param_1 = 0;
  DAT_00bc9768 = DAT_00bca2d8 + (int)param_2 * 0xe4 + iVar2;
  if (0 < param_4) {
    param_2 = &DAT_005cc8c0;
    do {
      iVar2 = 0;
      piVar1 = param_2;
      if (0 < param_3) {
        do {
          if (((&DAT_00af598c)[*piVar1 + DAT_00bc9768] & 0xeefed77f) != param_5) {
            return 0;
          }
          if ((param_6 != 0) && ((&DAT_00b7482c)[*piVar1 + DAT_00bc9768] != param_6 + -1)) {
            return 0;
          }
          iVar2 = iVar2 + 1;
          piVar1 = piVar1 + 2;
        } while (iVar2 < param_3);
      }
      param_1 = param_1 + 1;
      param_2 = param_2 + 0xc;
    } while (param_1 < param_4);
  }
  return 1;
}
