#include "restore_types.h"
#define DAT_00b67d1c (*(uint8_t *)0x00b67d1cu)
#define DAT_00bc9768 (*(uint32_t *)0x00bc9768u)
#define DAT_00bca2d0 (*(int32_t *)0x00bca2d0u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(int32_t *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(int32_t *)0x00d3b4e4u)

void FUN_00482b50(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = param_1 - param_4;
  iVar3 = param_1 + param_3 + -1 + param_4;
  _DAT_00d3b4e0 = param_2 - param_4;
  param_4 = param_3 + param_2 + -1 + param_4;
  if (iVar2 < 0) {
    iVar2 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  if (DAT_00bca2d0 <= iVar3) {
    iVar3 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= param_4) {
    param_4 = DAT_00bca2d4 + -1;
  }
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar2;
  if (_DAT_00d3b4e0 <= param_4) {
    iVar4 = (param_4 - _DAT_00d3b4e0) + 1;
    _DAT_00d3b4e0 = _DAT_00d3b4e0 + iVar4;
    do {
      _DAT_00d3b4e4 = iVar2;
      if (iVar2 <= iVar3) {
        iVar1 = (iVar3 - iVar2) + 1;
        _DAT_00d3b4e4 = iVar1 + iVar2;
        do {
          (&DAT_00b67d1c)[DAT_00bc9768] = (&DAT_00b67d1c)[DAT_00bc9768] & 0x7f;
          DAT_00bc9768 = DAT_00bc9768 + 1;
          iVar1 = iVar1 + -1;
        } while (iVar1 != 0);
      }
      DAT_00bc9768 = DAT_00bc9768 + (iVar2 - iVar3) + 0xe3;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  }
  return;
}
