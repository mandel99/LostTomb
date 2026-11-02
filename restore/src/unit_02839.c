#include "restore_types.h"
#define DAT_01266a90 (*(uint32_t *)0x01266a90u)
#define DAT_01267aa0 (*(int32_t *)0x01267aa0u)
extern int FUN_00562bea();

int __cdecl FUN_00562c8a(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = 0;
  iVar2 = 0;
  iVar4 = 0;
  if (0 < DAT_01267aa0) {
    do {
      iVar1 = *(int *)(DAT_01266a90 + iVar3 * 4);
      if ((iVar1 != 0) && ((*(uint *)(iVar1 + 0xc) & 0x83) != 0)) {
        if (param_1 == 1) {
          iVar1 = FUN_00562bea(iVar1);
          if (iVar1 != -1) {
            iVar2 = iVar2 + 1;
          }
        }
        else if ((param_1 == 0) && ((*(uint *)(iVar1 + 0xc) & 2) != 0)) {
          iVar1 = FUN_00562bea(iVar1);
          if (iVar1 == -1) {
            iVar4 = -1;
          }
        }
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < DAT_01267aa0);
  }
  if (param_1 != 1) {
    iVar2 = iVar4;
  }
  return iVar2;
}
