#include "restore_types.h"
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ebab20 (*(undefined4 *)0x00ebab20u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

undefined4 FUN_004800b0(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_1 - param_4;
  iVar2 = param_1 + param_3 + -1 + param_4;
  _DAT_00d3b4e0 = param_2 - param_4;
  param_4 = param_3 + param_2 + -1 + param_4;
  if (iVar1 < 0) {
    iVar1 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  if (DAT_00bca2d0 <= iVar2) {
    iVar2 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= param_4) {
    param_4 = DAT_00bca2d4 + -1;
  }
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar1;
  do {
    if (param_4 < _DAT_00d3b4e0) {
      return 0;
    }
    for (_DAT_00d3b4e4 = iVar1; _DAT_00d3b4e4 <= iVar2; _DAT_00d3b4e4 = _DAT_00d3b4e4 + 1) {
      if ((&DAT_00ebab20)[DAT_00bc9768] == '\0') {
        _DAT_00d3b4e0 = _DAT_00d3b4e0;
        _DAT_00d3b4e4 = _DAT_00d3b4e4;
        return 1;
      }
      DAT_00bc9768 = DAT_00bc9768 + 1;
    }
    _DAT_00d3b4e0 = _DAT_00d3b4e0 + 1;
    DAT_00bc9768 = DAT_00bc9768 + (iVar1 - iVar2) + 0xe3;
  } while( true );
}
