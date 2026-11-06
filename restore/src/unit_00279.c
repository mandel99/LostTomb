#include "restore_types.h"
#define DAT_0057c780 (*(int32_t *)0x0057c780u)
#define DAT_00702900 (*(undefined4 *)0x00702900u)
#define DAT_00702904 (*(undefined4 *)0x00702904u)
#define DAT_0070290c (*(undefined4 *)0x0070290cu)
#define DAT_007999d8 (*(uint8_t *)0x007999d8u)
#define DAT_007999e8 (*(int16_t *)0x007999e8u)
#define DAT_00799a18 (*(int16_t *)0x00799a18u)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)

void FUN_0041d410(int param_1,int param_2)

{
  short sVar1;
  int iVar2;
  int iVar3;
  
  if ((&DAT_007999d8)[param_1 * 0x108] != '\x01') {
    return;
  }
  sVar1 = (&DAT_007999e8)[param_1 * 0x84];
  iVar2 = (&DAT_0057c780)[sVar1];
  if (iVar2 < 1) {
    return;
  }
  if (sVar1 == 0x1f) {
    if (0 < (int)(&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276]) {
      return;
    }
    sVar1 = (&DAT_00799a18)[param_1 * 0x84];
  }
  else if (sVar1 == 0x1e) {
    if (0 < (int)(&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276]) {
      return;
    }
    sVar1 = (&DAT_00799a18)[param_1 * 0x84];
  }
  else if (sVar1 == 0x21) {
    if (0 < (int)(&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276]) {
      return;
    }
    sVar1 = (&DAT_00799a18)[param_1 * 0x84];
  }
  else if (sVar1 == 0x20) {
    if (0 < (int)(&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276]) {
      return;
    }
    sVar1 = (&DAT_00799a18)[param_1 * 0x84];
  }
  else {
    if (sVar1 != 0xe2) goto LAB_0041d527;
    if (0 < (int)(&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276]) {
      return;
    }
    sVar1 = (&DAT_00799a18)[param_1 * 0x84];
  }
  if (sVar1 < 1) {
    return;
  }
LAB_0041d527: ;
  iVar3 = (int32_t)((uint32_t)iVar2 * 0x20u);
  *(undefined4 *)(&DAT_00702900 + iVar3) = 1;
  *(uint32_t *)(&DAT_00702904 + iVar3) = *(uint32_t *)(&DAT_00702904 + iVar3) + 1u;
  param_2 = (int32_t)((uint32_t)iVar2 * 0x20u + (uint32_t)param_2);
  *(uint32_t *)(&DAT_0070290c + param_2) = *(uint32_t *)(&DAT_0070290c + param_2) + 1u;
  return;
}
