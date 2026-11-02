#include "restore_types.h"
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea3e34 (*(undefined4 *)0x00ea3e34u)
#define DAT_00ea3f68 (*(undefined4 *)0x00ea3f68u)
#define DAT_00ea5f0c (*(undefined4 *)0x00ea5f0cu)
extern void FUN_004e9400();

void FUN_004e91e0(int param_1,int param_2)

{
  short sVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar4 = param_1 * 0x49d8;
  *(int *)(&DAT_00ea3f68 + iVar4) = *(int *)(&DAT_00ea3f68 + iVar4) - param_2;
  FUN_004e9400(param_1,param_2);
  *(undefined4 *)(&DAT_00ea1894 + iVar4) = 0;
  psVar2 = (short *)(&DAT_00ea3e34 + iVar4);
  iVar3 = 100;
  do {
    sVar1 = *psVar2;
    psVar2 = psVar2 + 1;
    iVar5 = *(int *)(&DAT_00ea1894 + iVar4) + (int)sVar1;
    iVar3 = iVar3 + -1;
    *(int *)(&DAT_00ea1894 + iVar4) = iVar5;
  } while (iVar3 != 0);
  if ((int)(&DAT_00ea5f0c)[param_1 * 0x1276] < iVar5) {
    (&DAT_00ea5f0c)[param_1 * 0x1276] = iVar5;
  }
  return;
}
