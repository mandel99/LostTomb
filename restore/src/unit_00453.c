#include "restore_types.h"
#define DAT_0070e124 (*(undefined *)0x0070e124u)
#define DAT_0070e130 (*(undefined *)0x0070e130u)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799ac4 (*(undefined4 *)0x00799ac4u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ea1888 (*(undefined4 *)0x00ea1888u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0042fb30();

uint FUN_00430060(int param_1)

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
  int local_18;
  short *local_14;
  int local_c;
  
  FUN_0042fb30(param_1);
  DAT_0070e130 = 1;
  DAT_0070e124 = param_1;
  iVar3 = (100 - *(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8)) / 0x14;
  if (((DAT_0078d7c0 == 9) || (DAT_0078d7c0 == 10)) || (DAT_0078d7c0 == 0xb)) {
    iVar3 = iVar3 + 1;
  }
  if (iVar3 < 1) {
    return DAT_0078d7c0 & 0xffffff00;
  }
  if (4 < iVar3) {
    iVar3 = 4;
  }
  iVar9 = param_1 * 0x108;
  local_18 = (int)*(short *)((int)&DAT_007999dc + iVar9 + 2);
  iVar4 = local_18 + iVar3;
  local_18 = local_18 - iVar3;
  local_c = *(short *)(&DAT_007999e0 + iVar9) + iVar3;
  iVar3 = *(short *)(&DAT_007999e0 + iVar9) - iVar3;
  if (local_18 < 0) {
    local_18 = 0;
  }
  if (iVar3 < 0) {
    iVar3 = 0;
  }
  if (DAT_00bca2d0 <= iVar4) {
    iVar4 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= local_c) {
    local_c = DAT_00bca2d4 + -1;
  }
  iVar5 = DAT_00bca2d8 + iVar3 * 0xe4 + local_18;
  iVar8 = iVar3;
  for (; iVar3 <= local_c; iVar3 = iVar3 + 1) {
    if (local_18 <= iVar4) {
      local_14 = (short *)(&DAT_00b4e6fc + iVar5 * 2);
      iVar5 = iVar5 + 1 + (iVar4 - local_18);
      iVar8 = local_18;
      do {
        iVar1 = (int)*local_14;
        if (((iVar1 != 0) && ((&DAT_007999d8)[iVar1 * 0x108] != '\0')) &&
           ((*(char *)((int)&DAT_007999dc + iVar1 * 0x108 + 1) != '\0' &&
            ((&DAT_007999f4)[iVar1 * 0x84] != 0)))) {
          iVar6 = (int)*(short *)((int)&DAT_007999dc + iVar9 + 2);
          iVar2 = iVar8 - iVar6;
          iVar6 = iVar6 - iVar8;
          if (iVar2 <= iVar6) {
            iVar2 = iVar6;
          }
          iVar6 = iVar3 - *(short *)(&DAT_007999e0 + iVar9);
          iVar7 = *(short *)(&DAT_007999e0 + iVar9) - iVar3;
          if (iVar6 <= iVar7) {
            iVar6 = iVar7;
          }
          if (iVar6 < iVar2) {
            iVar6 = iVar2;
          }
          (&DAT_00799ac4)[iVar1 * 0x42] = iVar6;
        }
        iVar8 = iVar8 + 1;
        local_14 = local_14 + 1;
      } while (iVar8 <= iVar4);
    }
    iVar5 = iVar5 + (local_18 - iVar4) + 0xe3;
    iVar8 = local_c;
  }
  return CONCAT31((int3)((uint)iVar8 >> 8),1);
}
