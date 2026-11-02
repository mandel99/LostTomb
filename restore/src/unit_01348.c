#include "restore_types.h"
#define DAT_00bcd5ec (*(undefined4 *)0x00bcd5ecu)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00bcd7b4 (*(undefined4 *)0x00bcd7b4u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)

int FUN_004998f0(int param_1,int param_2)

{
  int iVar1;
  byte *pbVar2;
  int iVar3;
  char *pcVar4;
  int local_4;
  
  local_4 = 0;
  DAT_00bcd5ec = 10000;
  iVar3 = 1;
  pcVar4 = &DAT_00bcd7b4;
  do {
    if ((((pcVar4[-0x2a] == '\x01') && (*pcVar4 != '\x04')) && (*(short *)(pcVar4 + 0x4c) == 0)) &&
       ((10 < (byte)pcVar4[-0x2e] && ((byte)pcVar4[-0x2e] < 0xe)))) {
      iVar1 = (int)*(short *)(pcVar4 + -0x24);
      _DAT_00d3b4f8 = (int)*(short *)(pcVar4 + -0x22);
      if (iVar1 < param_1) {
        DAT_00d3b4fc = param_1 - iVar1;
      }
      else if (param_1 < iVar1) {
        DAT_00d3b4fc = iVar1 - param_1;
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
      iVar1 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar1 = DAT_00d3b4fc;
      }
      if (iVar1 < DAT_00bcd5ec) {
        DAT_00bcd5ec = iVar1;
        local_4 = iVar3;
      }
    }
    pcVar4 = pcVar4 + 0x184;
    iVar3 = iVar3 + 1;
  } while ((int)pcVar4 < 0xc8ad70);
  if (local_4 == 0) {
    iVar3 = 1;
    pbVar2 = &DAT_00bcd786;
    do {
      if (((pbVar2[4] == 1) && (pbVar2[0x2e] != 4)) && ((10 < *pbVar2 && (*pbVar2 < 0xe)))) {
        return iVar3;
      }
      pbVar2 = pbVar2 + 0x184;
      iVar3 = iVar3 + 1;
    } while ((int)pbVar2 < 0xc8ad42);
    local_4 = 0;
  }
  return local_4;
}
