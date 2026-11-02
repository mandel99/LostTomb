#include "restore_types.h"
#define DAT_00e34238 (*(undefined *)0x00e34238u)
#define DAT_00e34239 (*(char *)0x00e34239u)
#define _DAT_00d3b504 (*(undefined4 *)0x00d3b504u)
extern int FUN_004cc5c0();
extern void FUN_004cc720();

void FUN_004cce90(undefined4 param_1,char param_2,char *param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  char *pcVar1;
  char cVar2;
  int iVar3;
  
  iVar3 = 0;
  FUN_004cc5c0(param_1,1,0);
  cVar2 = DAT_00e34238;
  if (param_2 != '\0') {
    DAT_00e34238 = param_2;
    cVar2 = param_2;
  }
  while (cVar2 != '\0') {
    pcVar1 = &DAT_00e34239 + iVar3;
    iVar3 = iVar3 + 1;
    cVar2 = *pcVar1;
  }
  cVar2 = *param_3;
  while (cVar2 != '\0') {
    (&DAT_00e34238)[iVar3] = cVar2;
    pcVar1 = param_3 + 1;
    iVar3 = iVar3 + 1;
    param_3 = param_3 + 1;
    cVar2 = *pcVar1;
  }
  (&DAT_00e34238)[iVar3] = 0;
  FUN_004cc720(&DAT_00e34238,param_4,param_5,param_6,param_7,param_8);
  _DAT_00d3b504 = 0;
  return;
}
