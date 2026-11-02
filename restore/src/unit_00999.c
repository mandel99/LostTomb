#include "restore_types.h"
#define DAT_00ea450c (*(undefined4 *)0x00ea450cu)
#define DAT_00ea5cb0 (*(undefined4 *)0x00ea5cb0u)
#define DAT_00ea5cbc (*(undefined4 *)0x00ea5cbcu)
#define DAT_00ea5d58 (*(undefined4 *)0x00ea5d58u)
#define DAT_00ea5d5c (*(undefined4 *)0x00ea5d5cu)
#define DAT_00ea5df0 (*(undefined4 *)0x00ea5df0u)
#define DAT_00ea5df4 (*(undefined4 *)0x00ea5df4u)
#define DAT_00ea5df8 (*(undefined4 *)0x00ea5df8u)
#define DAT_00ea5e00 (*(undefined4 *)0x00ea5e00u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0046e880();

void FUN_0046ec70(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_00eaac28;
  (&DAT_00ea5df0)[DAT_00eaac28 * 0x1276] = 0;
  iVar2 = (&DAT_00ea450c)[iVar1 * 0x1276] - (&DAT_00ea5cbc)[iVar1 * 0x1276];
  if ((&DAT_00ea5cbc)[iVar1 * 0x1276] == 0) {
    if (iVar2 < 1) goto LAB_0046eca8;
  }
  else {
    if (iVar2 < 1) goto LAB_0046eca8;
    iVar2 = iVar2 + 1;
  }
  (&DAT_00ea5df0)[iVar1 * 0x1276] = iVar2;
LAB_0046eca8: ;
  FUN_0046e880();
  iVar1 = (&DAT_00ea5df0)[DAT_00eaac28 * 0x1276];
  if (7 < iVar1) {
    (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 1;
    return;
  }
  if (2 < (int)(&DAT_00ea5d5c)[DAT_00eaac28 * 0x1276]) {
    (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 2;
    return;
  }
  if (4 < (int)(&DAT_00ea5df8)[DAT_00eaac28 * 0x1276]) {
    (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 3;
    return;
  }
  if (4 < iVar1) {
    (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 4;
    return;
  }
  if (1 < (int)(&DAT_00ea5d5c)[DAT_00eaac28 * 0x1276]) {
    (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 5;
    return;
  }
  if (2 < (int)(&DAT_00ea5df8)[DAT_00eaac28 * 0x1276]) {
    (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 6;
    return;
  }
  if (2 < iVar1) {
    (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 7;
    return;
  }
  if ((&DAT_00ea5d58)[DAT_00eaac28 * 0x1276] == 0) {
    if (1 < iVar1) {
      (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 9;
      return;
    }
    if ((&DAT_00ea5df4)[DAT_00eaac28 * 0x1276] != 0) {
      (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 10;
      return;
    }
    if (iVar1 != 0) {
      (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 0xb;
      return;
    }
    if ((&DAT_00ea5e00)[DAT_00eaac28 * 0x1276] != 2) {
      if ((&DAT_00ea5e00)[DAT_00eaac28 * 0x1276] != 1) {
        (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 0;
        return;
      }
      (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 0xd;
      return;
    }
    (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 0xc;
    return;
  }
  (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] = 8;
  return;
}
