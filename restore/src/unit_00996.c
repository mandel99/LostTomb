#include "restore_types.h"
#define DAT_00ea5b4c (*(undefined4 *)0x00ea5b4cu)
#define DAT_00ea5c9c (*(undefined4 *)0x00ea5c9cu)
#define DAT_00ea5cb4 (*(undefined4 *)0x00ea5cb4u)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00ea5eb4 (*(undefined4 *)0x00ea5eb4u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)

void FUN_0046e880(void)

{
  int iVar1;
  
  if ((&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] != 0) {
    (&DAT_00ea5cb4)[DAT_00eaac28 * 0x1276] = 8;
    return;
  }
  if ((&DAT_00ea5eb4)[DAT_00eaac28 * 0x1276] != 0) {
    (&DAT_00ea5cb4)[DAT_00eaac28 * 0x1276] = 7;
    return;
  }
  if ((&DAT_00ea5c9c)[DAT_00eaac28 * 0x1276] != 0) {
    (&DAT_00ea5cb4)[DAT_00eaac28 * 0x1276] = 6;
    return;
  }
  iVar1 = *(int *)(&DAT_00ea5b4c + DAT_00eaac28 * 0x49d8);
  if (iVar1 < 10) {
    (&DAT_00ea5cb4)[DAT_00eaac28 * 0x1276] = 0;
    return;
  }
  if (iVar1 < 0x1e) {
    (&DAT_00ea5cb4)[DAT_00eaac28 * 0x1276] = 1;
    return;
  }
  if (iVar1 < 0x3c) {
    (&DAT_00ea5cb4)[DAT_00eaac28 * 0x1276] = 2;
    return;
  }
  if (iVar1 < 0x5a) {
    (&DAT_00ea5cb4)[DAT_00eaac28 * 0x1276] = 3;
    return;
  }
  if (iVar1 < 100) {
    (&DAT_00ea5cb4)[DAT_00eaac28 * 0x1276] = 4;
    return;
  }
  (&DAT_00ea5cb4)[DAT_00eaac28 * 0x1276] = 5;
  return;
}
