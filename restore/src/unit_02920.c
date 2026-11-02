#include "restore_types.h"
#define DAT_0060496c (*(undefined *)0x0060496cu)
#define DAT_01266434 (*(undefined *)0x01266434u)
#define DAT_01266960 (*(uint32_t *)0x01266960u)
#define DAT_01266a60 (*(uint32_t *)0x01266a60u)
#define _DAT_01266430 (*(undefined4 *)0x01266430u)

undefined4 FUN_005668d5(uint param_1,HANDLE param_2)

{
  int iVar1;
  DWORD nStdHandle;
  
  if (param_1 < DAT_01266a60) {
    iVar1 = (param_1 & 0x1f) * 8;
    if (*(int *)((&DAT_01266960)[(int)param_1 >> 5] + iVar1) == -1) {
      if (DAT_0060496c == 1) {
        if (param_1 == 0) {
          nStdHandle = 0xfffffff6;
        }
        else if (param_1 == 1) {
          nStdHandle = 0xfffffff5;
        }
        else {
          if (param_1 != 2) goto LAB_0056692b;
          nStdHandle = 0xfffffff4;
        }
        SetStdHandle(nStdHandle,param_2);
      }
LAB_0056692b: ;
      *(HANDLE *)((&DAT_01266960)[(int)param_1 >> 5] + iVar1) = param_2;
      return 0;
    }
  }
  DAT_01266434 = 0;
  _DAT_01266430 = 9;
  return 0xffffffff;
}
