#include "restore_types.h"
#define DAT_0077f77c (*(undefined4 *)0x0077f77cu)

undefined4 FUN_00443040(int param_1,uint param_2)

{
  int iVar1;
  
  iVar1 = 0;
  do {
    if ((byte)(&DAT_0077f77c)[iVar1 + param_1 * 0x62] == param_2) {
      return 1;
    }
    iVar1 = iVar1 + 1;
  } while (iVar1 < 8);
  return 0;
}
