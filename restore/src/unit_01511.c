#include "restore_types.h"
#define DAT_00a835fc (*(undefined4 *)0x00a835fcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

undefined4 FUN_004ba290(int param_1,int param_2,int param_3)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int local_8;
  int local_4;
  
  local_8 = param_1;
  iVar5 = param_1 - param_3;
  param_1 = param_1 + param_3;
  iVar2 = param_3 + param_2;
  _DAT_00d3b4e0 = param_2 - param_3;
  if (iVar5 < 0) {
    iVar5 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  if (DAT_00bca2d0 <= param_1) {
    param_1 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= iVar2) {
    iVar2 = DAT_00bca2d4 + -1;
  }
  local_4 = param_2;
  bVar1 = 0;
  iVar3 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar5;
  if (_DAT_00d3b4e0 <= iVar2) {
    do {
      _DAT_00d3b4e4 = iVar5;
      if (iVar5 <= param_1) {
        piVar4 = &DAT_00eed760 + iVar3;
        do {
          if ((0 < *piVar4) && (bVar1 < (byte)(&DAT_00a835fc)[iVar3])) {
            local_8 = _DAT_00d3b4e4;
            local_4 = _DAT_00d3b4e0;
            bVar1 = (&DAT_00a835fc)[iVar3];
          }
          _DAT_00d3b4e4 = _DAT_00d3b4e4 + 1;
          iVar3 = iVar3 + 1;
          piVar4 = piVar4 + 1;
        } while (_DAT_00d3b4e4 <= param_1);
      }
      _DAT_00d3b4e0 = _DAT_00d3b4e0 + 1;
      iVar3 = iVar3 + (iVar5 - param_1) + 0xe3;
    } while (_DAT_00d3b4e0 <= iVar2);
    if (bVar1 != 0) {
      _DAT_00d3b4e0 = local_4;
      _DAT_00d3b4e4 = local_8;
      return 1;
    }
  }
  return 0;
}
