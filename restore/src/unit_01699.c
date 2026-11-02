#include "restore_types.h"
#define DAT_00e34238 (*(undefined *)0x00e34238u)
#define _DAT_00d3b504 (*(undefined4 *)0x00d3b504u)
extern void FUN_004cc720();

void FUN_004ccd60(char *param_1,char *param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  char *pcVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = 0;
  if (param_1 != (char *)0x0) {
    if (*param_1 != '\0') {
      iVar4 = (int)&DAT_00e34238 - (int)param_1;
      do {
        if (0xfe < iVar3) break;
        iVar3 = iVar3 + 1;
        param_1[iVar4] = *param_1;
        pcVar1 = param_1 + 1;
        param_1 = param_1 + 1;
      } while (*pcVar1 != '\0');
    }
    (&DAT_00e34238)[iVar3] = 0x20;
    iVar3 = iVar3 + 1;
  }
  if (param_2 != (char *)0x0) {
    cVar2 = *param_2;
    for (; (cVar2 != '\0' && (iVar3 < 0xff)); iVar3 = iVar3 + 1) {
      (&DAT_00e34238)[iVar3] = *param_2;
      cVar2 = param_2[1];
      param_2 = param_2 + 1;
    }
  }
  (&DAT_00e34238)[iVar3] = 0;
  FUN_004cc720(&DAT_00e34238,param_3,param_4,param_5,param_6,param_7);
  _DAT_00d3b504 = 0;
  return;
}
