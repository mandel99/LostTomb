#include "restore_types.h"
#define DAT_00d3b4e8 (*(undefined *)0x00d3b4e8u)
#define DAT_00d3b4f0 (*(undefined *)0x00d3b4f0u)
#define DAT_00d3b4f4 (*(undefined *)0x00d3b4f4u)
extern void FUN_0040fb10();
extern void FUN_004cd850();
extern bool FUN_004cdc30();

void FUN_004cd710(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  DAT_00d3b4e8 = 0;
  iVar1 = param_1 + param_3;
  DAT_00d3b4f0 = DAT_00d3b4f4;
  for (iVar4 = param_1; iVar4 < iVar1; iVar4 = iVar4 + 1) {
    iVar2 = FUN_004cdc30(iVar4,param_2);
    if (iVar2 != 0) {
      FUN_0040fb10(iVar4,param_2);
    }
    FUN_004cd850();
  }
  iVar4 = param_1;
  if (param_1 < iVar1) {
    iVar2 = param_2 + -1 + param_4;
    do {
      iVar3 = FUN_004cdc30(iVar4,iVar2);
      if (iVar3 != 0) {
        FUN_0040fb10(iVar4,iVar2);
      }
      FUN_004cd850();
      iVar4 = iVar4 + 1;
    } while (iVar4 < iVar1);
  }
  param_4 = param_4 + param_2;
  for (iVar2 = param_2; iVar2 < param_4; iVar2 = iVar2 + 1) {
    iVar3 = FUN_004cdc30(param_1,iVar2);
    if (iVar3 != 0) {
      FUN_0040fb10(param_1,iVar2);
    }
    FUN_004cd850();
  }
  if (param_2 < param_4) {
    do {
      iVar2 = FUN_004cdc30(param_3 + -1 + iVar4,param_2);
      if (iVar2 != 0) {
        FUN_0040fb10(iVar1 + -1,param_2);
      }
      FUN_004cd850();
      param_2 = param_2 + 1;
    } while (param_2 < param_4);
  }
  return;
}
