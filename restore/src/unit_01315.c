#include "restore_types.h"
#define DAT_00bca524 (*(undefined4 *)0x00bca524u)
#define DAT_00bca6b0 (*(undefined4 *)0x00bca6b0u)
#define DAT_00bca6b4 (*(undefined4 *)0x00bca6b4u)
#define DAT_00bcd5ec (*(undefined4 *)0x00bcd5ecu)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00bca844 (*(undefined4 *)0x00bca844u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern int FUN_00518b30();

undefined4 FUN_0048e820(int param_1,int param_2,int param_3)

{
  byte bVar1;
  undefined4 uVar2;
  int iVar3;
  byte *pbVar4;
  int local_4;
  
  _DAT_00bca844 = 0;
  DAT_00bcd5ec = 10000;
  local_4 = 1;
  pbVar4 = &DAT_00bcd786;
  do {
    if ((pbVar4[4] == 1) && (*(short *)(pbVar4 + 0x7a) == 0)) {
      if (99 < _DAT_00bca844) break;
      bVar1 = *pbVar4;
      if ((bVar1 == 0x29) && (pbVar4[0x2e] == 0xb)) {
        iVar3 = (int)*(short *)(pbVar4 + 10);
        _DAT_00d3b4f8 = (int)*(short *)(pbVar4 + 0xc);
        if (iVar3 < param_1) {
          DAT_00d3b4fc = param_1 - iVar3;
        }
        else if (param_1 < iVar3) {
          DAT_00d3b4fc = iVar3 - param_1;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (_DAT_00d3b4f8 < param_2) {
          _DAT_00d3b4f8 = param_2 - _DAT_00d3b4f8;
        }
        else if (param_2 < _DAT_00d3b4f8) {
          _DAT_00d3b4f8 = _DAT_00d3b4f8 - param_2;
        }
        else {
          _DAT_00d3b4f8 = 0;
        }
        iVar3 = _DAT_00d3b4f8;
        if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
          iVar3 = DAT_00d3b4fc;
        }
      }
      else if (((bVar1 < 0x2b) || (0x39 < bVar1)) && (bVar1 != 99)) {
        if ((((bVar1 != 0x52) && (bVar1 != 0x53)) && (bVar1 != 0x54)) &&
           (((bVar1 != 0x67 && (bVar1 != 0x68)) && ((bVar1 != 0x66 && (bVar1 != 0x6d))))))
        goto LAB_0048e98d;
        iVar3 = (int)*(short *)(pbVar4 + 10);
        _DAT_00d3b4f8 = (int)*(short *)(pbVar4 + 0xc);
        if (iVar3 < param_1) {
          DAT_00d3b4fc = param_1 - iVar3;
        }
        else if (param_1 < iVar3) {
          DAT_00d3b4fc = iVar3 - param_1;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (_DAT_00d3b4f8 < param_2) {
          _DAT_00d3b4f8 = param_2 - _DAT_00d3b4f8;
        }
        else if (param_2 < _DAT_00d3b4f8) {
          _DAT_00d3b4f8 = _DAT_00d3b4f8 - param_2;
        }
        else {
          _DAT_00d3b4f8 = 0;
        }
        iVar3 = _DAT_00d3b4f8;
        if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
          iVar3 = DAT_00d3b4fc;
        }
        iVar3 = iVar3 * 3;
      }
      else {
        iVar3 = (int)*(short *)(pbVar4 + 10);
        _DAT_00d3b4f8 = (int)*(short *)(pbVar4 + 0xc);
        if (iVar3 < param_1) {
          DAT_00d3b4fc = param_1 - iVar3;
        }
        else if (param_1 < iVar3) {
          DAT_00d3b4fc = iVar3 - param_1;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (_DAT_00d3b4f8 < param_2) {
          _DAT_00d3b4f8 = param_2 - _DAT_00d3b4f8;
        }
        else if (param_2 < _DAT_00d3b4f8) {
          _DAT_00d3b4f8 = _DAT_00d3b4f8 - param_2;
        }
        else {
          _DAT_00d3b4f8 = 0;
        }
        iVar3 = _DAT_00d3b4f8;
        if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
          iVar3 = DAT_00d3b4fc;
        }
        iVar3 = iVar3 * 2;
      }
      if (iVar3 < param_3) {
        uVar2 = *(undefined4 *)(pbVar4 + 0x16);
        (&DAT_00bca6b4)[_DAT_00bca844] = local_4;
        (&DAT_00bca524)[_DAT_00bca844] = uVar2;
        _DAT_00bca844 = _DAT_00bca844 + 1;
      }
    }
LAB_0048e98d: ;
    pbVar4 = pbVar4 + 0x184;
    local_4 = local_4 + 1;
  } while ((int)pbVar4 < 0xc8ad42);
  if (_DAT_00bca844 != 0) {
    iVar3 = FUN_00518b30(param_1,param_2,&DAT_00bca524,_DAT_00bca844);
    if (0 < iVar3) {
      return *(undefined4 *)(&DAT_00bca6b0 + iVar3 * 4);
    }
  }
  return 0;
}
