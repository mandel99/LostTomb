#include "restore_types.h"
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
extern int FUN_004cbf30();
extern void FUN_004cc110();

int FUN_004d5f80(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = 1;
  iVar3 = 1;
  FUN_004cc110(param_1 + 1,0);
  iVar1 = DAT_00e20768;
  FUN_004cc110(param_1,1);
  do {
    iVar2 = FUN_004cbf30(DAT_00e20768,DAT_00f56f8c);
    if (iVar4 < iVar2) {
      iVar4 = iVar2;
    }
    iVar3 = iVar3 + 1;
    FUN_004cc110(param_1,iVar3);
  } while (iVar1 != DAT_00e20768);
  return iVar4;
}
