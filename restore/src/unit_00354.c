#include "restore_types.h"
#define PTR_DAT_005ad218 (*(unsigned char * *)0x005ad218u)
extern void FUN_0041f910();
extern int FUN_0053e590();

void FUN_00420d60(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int in_ECX;
  undefined **ppuVar3;
  int local_4;
  
  iVar2 = param_2;
  iVar1 = param_1;
  ppuVar3 = &PTR_DAT_005ad218;
  local_4 = in_ECX;
  do {
    FUN_0041f910(&local_4,&param_1,&param_2);
    if (param_2 != 0) {
      FUN_0053e590(param_2,local_4 + iVar1,param_1 + iVar2);
    }
    ppuVar3 = ppuVar3 + 4;
  } while ((int)ppuVar3 < 0x5ad3c8);
  return;
}
