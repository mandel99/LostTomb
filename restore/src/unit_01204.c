#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

undefined4 FUN_00482810(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = param_1 - param_4;
  iVar3 = param_1 + param_3 + -1 + param_4;
  _DAT_00d3b4e0 = param_2 - param_4;
  param_4 = param_3 + param_2 + -1 + param_4;
  if (iVar4 < 0) {
    iVar4 = 0;
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
  iVar1 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar4;
  do {
    if (param_4 < _DAT_00d3b4e0) {
      return 0;
    }
    _DAT_00d3b4e4 = iVar4;
    if (iVar4 <= iVar3) {
      piVar2 = &DAT_00af598c + iVar1;
      do {
        if ((iVar1 != param_5) && (*piVar2 == 0)) {
          _DAT_00d3b4e0 = _DAT_00d3b4e0;
          return 1;
        }
        _DAT_00d3b4e4 = _DAT_00d3b4e4 + 1;
        iVar1 = iVar1 + 1;
        piVar2 = piVar2 + 1;
      } while (_DAT_00d3b4e4 <= iVar3);
    }
    _DAT_00d3b4e0 = _DAT_00d3b4e0 + 1;
    iVar1 = iVar1 + (iVar4 - iVar3) + 0xe3;
  } while( true );
}
