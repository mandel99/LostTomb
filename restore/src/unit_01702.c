#include "restore_types.h"
#define DAT_00e34238 (*(undefined *)0x00e34238u)
#define DAT_00e34239 (*(char *)0x00e34239u)
#define _DAT_00d3b504 (*(undefined4 *)0x00d3b504u)
extern int FUN_004cc5c0();
extern void FUN_004cc720();

void FUN_004ccf20(undefined4 param_1,char *param_2,char *param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  char *pcVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  
  iVar4 = 0;
  iVar2 = 0;
  cVar3 = *param_2;
  while (cVar3 != '\0') {
    iVar5 = iVar2 + 1;
    iVar2 = iVar2 + 1;
    cVar3 = param_2[iVar5];
  }
  iVar2 = FUN_004cc5c0(param_1,iVar2 + 1,0);
  cVar3 = *param_2;
  if (cVar3 != '\0') {
    iVar5 = (int)&DAT_00e34238 - (int)param_2;
    do {
      param_2[iVar5] = cVar3;
      cVar3 = param_2[1];
      iVar4 = iVar4 + 1;
      param_2 = param_2 + 1;
    } while (cVar3 != '\0');
  }
  (&DAT_00e34238)[iVar4] = 0x20;
  (&DAT_00e34238)[iVar2] = 0x20;
  cVar3 = *param_3;
  while (cVar3 != '\0') {
    (&DAT_00e34239)[iVar2] = cVar3;
    pcVar1 = param_3 + 1;
    param_3 = param_3 + 1;
    iVar2 = iVar2 + 1;
    cVar3 = *pcVar1;
  }
  (&DAT_00e34239)[iVar2] = 0;
  FUN_004cc720(&DAT_00e34238,param_4,param_5,param_6,param_7,param_8);
  _DAT_00d3b504 = 0;
  return;
}
