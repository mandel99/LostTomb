#include "restore_types.h"
#define DAT_00d3b4c0 (*(undefined *)0x00d3b4c0u)
#define DAT_00d3b4c4 (*(undefined *)0x00d3b4c4u)
#define DAT_00d3b4c8 (*(undefined *)0x00d3b4c8u)
#define DAT_00d3b4cc (*(undefined *)0x00d3b4ccu)
#define DAT_00d3b4d0 (*(undefined *)0x00d3b4d0u)
#define DAT_00d3b4d4 (*(undefined *)0x00d3b4d4u)
#define DAT_00d3b4d8 (*(undefined *)0x00d3b4d8u)
#define DAT_00d3b4dc (*(undefined *)0x00d3b4dcu)

void FUN_004cd3e0(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  bool bVar2;
  bool bVar3;
  
  DAT_00d3b4dc = 1;
  DAT_00d3b4d8 = 1;
  if (param_3 < param_1) {
    DAT_00d3b4d0 = param_1 - param_3;
  }
  else {
    DAT_00d3b4d0 = param_3 - param_1;
  }
  if (param_4 < param_2) {
    DAT_00d3b4d4 = param_2 - param_4;
  }
  else {
    DAT_00d3b4d4 = param_4 - param_2;
  }
  if (DAT_00d3b4d0 < DAT_00d3b4d4) {
    bVar3 = SBORROW4(param_2,param_4);
    iVar1 = param_2 - param_4;
    bVar2 = param_2 == param_4;
  }
  else {
    bVar3 = SBORROW4(param_1,param_3);
    iVar1 = param_1 - param_3;
    bVar2 = param_1 == param_3;
  }
  DAT_00d3b4c4 = param_4;
  DAT_00d3b4c0 = param_3;
  if (!bVar2 && bVar3 == iVar1 < 0) {
    DAT_00d3b4c4 = param_2;
    DAT_00d3b4c0 = param_1;
    param_2 = param_4;
    param_1 = param_3;
  }
  if (DAT_00d3b4c0 < param_1) {
    DAT_00d3b4d8 = 0xffffffff;
  }
  if (DAT_00d3b4c4 < param_2) {
    DAT_00d3b4dc = 0xffffffff;
  }
  DAT_00d3b4c8 = param_1;
  DAT_00d3b4cc = param_2;
  return;
}
