#include "restore_types.h"
#define DAT_00a76aec (*(undefined4 *)0x00a76aecu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ea5f1c (*(undefined4 *)0x00ea5f1cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

undefined4 FUN_0047feb0(int param_1,int param_2,undefined4 *param_3,int param_4,int param_5)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  iVar2 = param_1 + (int)param_3;
  param_1 = param_1 - param_4;
  iVar2 = iVar2 + -1 + param_4;
  _DAT_00d3b4e0 = param_2 - param_4;
  param_2 = (int)param_3 + param_2 + -1 + param_4;
  if (param_1 < 0) {
    param_1 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  param_4 = iVar2;
  if (DAT_00bca2d0 <= iVar2) {
    param_4 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= param_2) {
    param_2 = DAT_00bca2d4 + -1;
  }
  local_14 = 0xc;
  local_10 = param_1;
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + param_1;
  local_c = _DAT_00d3b4e0;
  local_8 = DAT_00bc9768;
  if (_DAT_00d3b4e0 <= param_2) {
    do {
      _DAT_00d3b4e4 = param_1;
      if (param_1 <= param_4) {
        param_3 = &DAT_00af598c + DAT_00bc9768;
        do {
          if ((((byte)*param_3 & 0x44) == 0x40) && (param_5 == 0)) {
            iVar2 = 0;
            puVar1 = &DAT_00ea5f1c + DAT_00eaac28 * 0x1276;
            do {
              iVar3 = iVar2;
              if (*puVar1 == (uint)(byte)(&DAT_00a76aec)[DAT_00bc9768]) break;
              iVar2 = iVar2 + 1;
              puVar1 = puVar1 + 2;
              iVar3 = 0xb;
            } while (iVar2 < 5);
            if (iVar3 < local_14) {
              local_14 = iVar3;
              local_10 = _DAT_00d3b4e4;
              local_c = _DAT_00d3b4e0;
              local_8 = DAT_00bc9768;
            }
          }
          _DAT_00d3b4e4 = _DAT_00d3b4e4 + 1;
          DAT_00bc9768 = DAT_00bc9768 + 1;
          param_3 = param_3 + 1;
        } while (_DAT_00d3b4e4 <= param_4);
      }
      _DAT_00d3b4e0 = _DAT_00d3b4e0 + 1;
      DAT_00bc9768 = DAT_00bc9768 + (param_1 - param_4) + 0xe3;
    } while (_DAT_00d3b4e0 <= param_2);
    if (local_14 < 0xc) {
      DAT_00bc9768 = local_8;
      _DAT_00d3b4e4 = local_10;
      _DAT_00d3b4e0 = local_c;
      return 1;
    }
  }
  return 0;
}
