#include "restore_types.h"
#define DAT_0078d7f8 (*(undefined4 *)0x0078d7f8u)
#define DAT_0078d7fc (*(undefined *)0x0078d7fcu)
#define DAT_0078dd90 (*(undefined4 *)0x0078dd90u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_00799a08 (*(undefined4 *)0x00799a08u)
#define DAT_00799a94 (*(undefined4 *)0x00799a94u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_0078d800 (*(undefined4 *)0x0078d800u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)

int FUN_00469a50(int param_1,int param_2)

{
  int iVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  DAT_0078d7f8 = 10000;
  local_10 = 10000;
  local_8 = 0;
  _DAT_0078d800 = 0;
  local_4 = 0;
  iVar1 = 0;
  if (0 < DAT_0078d7fc) {
    psVar2 = &DAT_0078dd90;
    local_c = DAT_0078d7fc;
    _DAT_0078d800 = DAT_0078d7fc;
    do {
      iVar4 = (int)*psVar2;
      psVar2 = psVar2 + 1;
      iVar1 = iVar4 * 0x108;
      if (((((&DAT_007999d8)[iVar1] == '\x01') && ((&DAT_007999e8)[iVar4 * 0x84] == 99)) &&
          (*(char *)((int)&DAT_00799a94 + iVar1 + 1) == '\0')) &&
         ((&DAT_007999f8)[iVar4 * 0x84] != 0)) {
        iVar3 = (int)*(short *)((int)&DAT_007999dc + iVar1 + 2);
        _DAT_00d3b4f8 = (int)*(short *)(&DAT_007999e0 + iVar1);
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
        if (*(short *)(&DAT_00799a08 + iVar1) == 0) {
          if (iVar3 < DAT_0078d7f8) {
            DAT_0078d7f8 = iVar3;
            local_8 = iVar4;
          }
        }
        else if (iVar3 < local_10) {
          local_10 = iVar3;
          local_4 = iVar4;
        }
      }
      local_c = local_c + -1;
    } while (local_c != 0);
    iVar1 = local_8;
    if ((local_8 == 0) && (local_10 < 3)) {
      iVar1 = local_4;
    }
  }
  return iVar1;
}
