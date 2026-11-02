#include "restore_types.h"
extern int FUN_004cbe60();

int FUN_004cbdd0(char *param_1,char *param_2,int param_3)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  char *pcVar4;
  int iVar5;
  
  uVar3 = 0xffffffff;
  pcVar4 = param_2;
  do {
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    cVar1 = *pcVar4;
    pcVar4 = pcVar4 + 1;
  } while (cVar1 != '\0');
  iVar5 = 0;
  if (0 < param_3) {
    do {
      if ((*param_1 == *param_2) && (iVar2 = FUN_004cbe60(param_1,param_2,~uVar3 - 1), iVar2 == 0))
      {
        return iVar5;
      }
      iVar5 = iVar5 + 1;
      param_1 = param_1 + 1;
    } while (iVar5 < param_3);
  }
  return 0;
}
