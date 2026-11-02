#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

void FUN_004824b0(int param_1,int param_2,int param_3,int param_4,uint param_5)

{
  int iVar1;
  uint *puVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = param_1 - param_4;
  iVar4 = param_1 + param_3 + -1 + param_4;
  _DAT_00d3b4e0 = param_2 - param_4;
  param_4 = param_3 + param_2 + -1 + param_4;
  if (iVar3 < 0) {
    iVar3 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  if (DAT_00bca2d0 <= iVar4) {
    iVar4 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= param_4) {
    param_4 = DAT_00bca2d4 + -1;
  }
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar3;
  if (_DAT_00d3b4e0 <= param_4) {
    param_1 = (param_4 - _DAT_00d3b4e0) + 1;
    _DAT_00d3b4e0 = param_1 + _DAT_00d3b4e0;
    do {
      _DAT_00d3b4e4 = iVar3;
      if (iVar3 <= iVar4) {
        puVar2 = &DAT_00af598c + DAT_00bc9768;
        iVar1 = (iVar4 - iVar3) + 1;
        DAT_00bc9768 = DAT_00bc9768 + iVar1;
        _DAT_00d3b4e4 = iVar1 + iVar3;
        do {
          *puVar2 = *puVar2 | param_5;
          puVar2 = puVar2 + 1;
          iVar1 = iVar1 + -1;
        } while (iVar1 != 0);
      }
      DAT_00bc9768 = DAT_00bc9768 + (iVar3 - iVar4) + 0xe3;
      param_1 = param_1 + -1;
    } while (param_1 != 0);
  }
  return;
}
