#include "restore_types.h"
#define DAT_01266434 (*(undefined *)0x01266434u)
#define DAT_01266960 (*(uint32_t *)0x01266960u)
#define DAT_01266a60 (*(uint32_t *)0x01266a60u)
#define _DAT_01266430 (*(undefined4 *)0x01266430u)
extern DWORD FUN_005662e3();

int FUN_0056b1b6(uint param_1)

{
  int iVar1;
  int iVar2;
  
  if ((param_1 < DAT_01266a60) &&
     ((*(byte *)((&DAT_01266960)[(int)param_1 >> 5] + 4 + (param_1 & 0x1f) * 8) & 1) != 0)) {
    iVar1 = FUN_005662e3(param_1,0,1);
    iVar2 = -1;
    if (iVar1 != -1) {
      iVar2 = FUN_005662e3(param_1,0,2);
      if (iVar1 != iVar2) {
        FUN_005662e3(param_1,iVar1,0);
      }
    }
    return iVar2;
  }
  DAT_01266434 = 0;
  _DAT_01266430 = 9;
  return -1;
}
