#include "restore_types.h"
#define DAT_00705ea8 (*(undefined4 *)0x00705ea8u)
#define DAT_00705eb0 (*(undefined4 *)0x00705eb0u)
#define DAT_00705eb4 (*(undefined4 *)0x00705eb4u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define DAT_00eadcd4 (*(undefined *)0x00eadcd4u)
#define DAT_00eadcd8 (*(undefined *)0x00eadcd8u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern int FUN_00475b50();

void FUN_0050f0f0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int local_20;
  short *local_1c;
  int local_14;
  
  local_20 = (int)*(short *)((int)&DAT_007999dc + DAT_00eadd6c * 0x108 + 2);
  iVar4 = local_20 + 6;
  local_20 = local_20 + -6;
  local_14 = *(short *)(&DAT_007999e0 + DAT_00eadd6c * 0x108) + 6;
  iVar1 = *(short *)(&DAT_007999e0 + DAT_00eadd6c * 0x108) + -6;
  DAT_00eadcd8 = 0;
  DAT_00eadcd4 = 0;
  if (local_20 < 0) {
    local_20 = 0;
  }
  if (iVar1 < 0) {
    iVar1 = 0;
  }
  if (DAT_00bca2d0 <= iVar4) {
    iVar4 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= local_14) {
    local_14 = DAT_00bca2d4 + -1;
  }
  iVar5 = DAT_00bca2d8 + iVar1 * 0xe4 + local_20;
  iVar7 = DAT_00eadd6c;
  for (; iVar1 <= local_14; iVar1 = iVar1 + 1) {
    if (local_20 <= iVar4) {
      local_1c = (short *)(&DAT_00b4e6fc + iVar5 * 2);
      iVar5 = iVar5 + 1 + (iVar4 - local_20);
      iVar9 = local_20;
      do {
        iVar2 = (int)*local_1c;
        iVar3 = DAT_00eadcd4;
        if ((((0 < iVar2) && ((&DAT_007999d8)[iVar2 * 0x108] == '\x01')) && (iVar2 != iVar7)) &&
           ((*(char *)((int)&DAT_007999dc + iVar2 * 0x108 + 1) == '\0' ||
            ((short)(&DAT_007999e8)[iVar2 * 0x84] < (short)(&DAT_007999e8)[iVar7 * 0x84])))) {
          iVar6 = (short)(&DAT_007999e8)[iVar2 * 0x84] * 0x28;
          iVar10 = *(int *)(&DAT_00705ea8 + iVar6);
          if (iVar10 < 0) {
            _DAT_00d3b4f8 = (int)*(short *)(&DAT_007999e0 + iVar7 * 0x108);
            iVar7 = (int)*(short *)((int)&DAT_007999dc + iVar7 * 0x108 + 2);
            if (iVar7 < iVar9) {
              DAT_00d3b4fc = iVar9 - iVar7;
            }
            else if (iVar9 < iVar7) {
              DAT_00d3b4fc = iVar7 - iVar9;
            }
            else {
              DAT_00d3b4fc = 0;
            }
            if (_DAT_00d3b4f8 < iVar1) {
              _DAT_00d3b4f8 = iVar1 - _DAT_00d3b4f8;
            }
            else if (iVar1 < _DAT_00d3b4f8) {
              _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar1;
            }
            else {
              _DAT_00d3b4f8 = 0;
            }
            iVar8 = _DAT_00d3b4f8;
            if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
              iVar8 = DAT_00d3b4fc;
            }
            iVar7 = DAT_00eadd6c;
            if (iVar8 <= *(int *)(&DAT_00705eb4 + iVar6)) {
              if (1 < iVar8) {
                iVar10 = iVar10 + (iVar8 + -1) * *(int *)(&DAT_00705eb0 + iVar6);
              }
              if ((iVar10 < DAT_00eadcd8) &&
                 (iVar3 = iVar2, DAT_00eadcd8 = iVar10, (&DAT_007999e8)[iVar2 * 0x84] == 0x36)) {
                iVar3 = FUN_00475b50(iVar2);
                iVar7 = DAT_00eadd6c;
              }
            }
          }
        }
        DAT_00eadcd4 = iVar3;
        iVar9 = iVar9 + 1;
        local_1c = local_1c + 1;
      } while (iVar9 <= iVar4);
    }
    iVar5 = iVar5 + (local_20 - iVar4) + 0xe3;
  }
  return;
}
