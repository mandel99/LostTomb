#include "restore_types.h"
#define DAT_00784b74 (*(undefined *)0x00784b74u)
#define DAT_00784b84 (*(undefined *)0x00784b84u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)

undefined4 FUN_00480c60(int param_1)

{
  short *psVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int local_8;
  int local_4;
  
  psVar1 = &DAT_00784b74;
  iVar5 = 0;
  iVar2 = 0;
  do {
    if (0 < *psVar1) {
      iVar2 = iVar2 + 1;
    }
    psVar1 = psVar1 + 1;
  } while ((int)psVar1 < 0x784b84);
  if (iVar2 < 1) {
    return 0;
  }
  local_8 = 10000;
  local_4 = 0;
  psVar1 = &DAT_00784b74;
  do {
    if (0 < *psVar1) {
      iVar3 = (int)*psVar1;
      iVar4 = (int)psVar1[8];
      _DAT_00d3b4f8 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
      iVar2 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
      if (iVar3 < iVar2) {
        DAT_00d3b4fc = iVar2 - iVar3;
      }
      else if (iVar2 < iVar3) {
        DAT_00d3b4fc = iVar3 - iVar2;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar4 < _DAT_00d3b4f8) {
        _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar4;
      }
      else if (_DAT_00d3b4f8 < iVar4) {
        _DAT_00d3b4f8 = iVar4 - _DAT_00d3b4f8;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar2 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar2 = DAT_00d3b4fc;
      }
      if (iVar2 < local_8) {
        local_8 = iVar2;
        local_4 = iVar5;
      }
    }
    psVar1 = psVar1 + 1;
    iVar5 = iVar5 + 1;
  } while ((int)psVar1 < 0x784b84);
  if (9999 < local_8) {
    return 0;
  }
  _DAT_00d3b4e4 = (uint)*(byte *)(&DAT_00784b74 + local_4);
  _DAT_00d3b4e0 = (uint)*(byte *)(&DAT_00784b84 + local_4);
  return 1;
}
