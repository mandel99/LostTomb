#include "restore_types.h"
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern longlong FUN_0056133c();

int FUN_00489050(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  
  DAT_00bc9768 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  if (0 < param_4) {
    param_2 = param_4;
    do {
      iVar1 = param_3;
      if (0 < param_3) {
        do {
          iVar1 = iVar1 + -1;
        } while (iVar1 != 0);
      }
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  iVar1 = FUN_0056133c();
  return iVar1 + 1;
}
