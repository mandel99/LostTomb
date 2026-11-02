#include "restore_types.h"
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea3f7c (*(undefined4 *)0x00ea3f7cu)
#define DAT_00ea5eac (*(undefined4 *)0x00ea5eacu)
#define DAT_00ea5eb0 (*(undefined4 *)0x00ea5eb0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004cde60();

int FUN_004e8ec0(void)

{
  int iVar1;
  
  iVar1 = FUN_004cde60((&DAT_00ea5eac)[DAT_00eaac28 * 0x1276],
                       *(undefined4 *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8));
  if (0 < *(int *)(&DAT_00ea3f7c + DAT_00eaac28 * 0x49d8)) {
    if (0x38 < iVar1) {
      return 0;
    }
    if (0x27 < iVar1) {
      return -3;
    }
    if (0x19 < iVar1) {
      return -4;
    }
    return (9 < iVar1) - 6;
  }
  if ((int)(&DAT_00ea5eb0)[DAT_00eaac28 * 0x1276] < 1) {
    if (0x27 < iVar1) {
      return 0;
    }
    if (0x19 < iVar1) {
      return -1;
    }
    return (9 < iVar1) - 3;
  }
  if (0x38 < iVar1) {
    return 0;
  }
  if (0x27 < iVar1) {
    return -2;
  }
  if (0x19 < iVar1) {
    return -3;
  }
  return (9 < iVar1) - 5;
}
