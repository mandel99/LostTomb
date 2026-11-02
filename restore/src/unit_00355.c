#include "restore_types.h"
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define PTR_DAT_005ad218 (*(unsigned char * *)0x005ad218u)
extern void FUN_00417640();

void FUN_00420dc0(int param_1)

{
  bool bVar1;
  undefined **ppuVar2;
  
  bVar1 = false;
  ppuVar2 = &PTR_DAT_005ad218;
  do {
    if (0x5ad3c7 < (int)ppuVar2) {
      if (!bVar1) {
        FUN_00417640(param_1);
      }
      return;
    }
    if ((ppuVar2[-2] == (undefined *)(&DAT_00799a30)[param_1 * 0x42]) &&
       (0 < *(int *)(*ppuVar2 + 0xc))) {
      bVar1 = true;
    }
    ppuVar2 = ppuVar2 + 4;
  } while (!bVar1);
  return;
}
