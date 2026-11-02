#include "restore_types.h"
#define DAT_00bca32c (*(char *)0x00bca32cu)
#define DAT_00bca32d (*(undefined1 *)0x00bca32du)
#define DAT_00f56c6c (*(undefined *)0x00f56c6cu)
#define DAT_00f56c74 (*(undefined *)0x00f56c74u)
#define DAT_00f56c7c (*(undefined *)0x00f56c7cu)
#define DAT_00f56ccc (*(undefined *)0x00f56cccu)
#define DAT_00f57284 (*(undefined *)0x00f57284u)
#define DAT_00f5728c (*(undefined *)0x00f5728cu)
#define DAT_00f57294 (*(undefined *)0x00f57294u)
#define DAT_00f5729c (*(undefined *)0x00f5729cu)

int FUN_00487d30(int param_1,char param_2)

{
  int iVar1;
  
  if (DAT_00bca32c == '\0') {
    iVar1 = 0;
  }
  else {
    iVar1 = (-(uint)(DAT_00bca32d != '\0') & 4) + 4;
  }
  if (param_2 == '\0') {
    if (param_1 == 0) {
      return iVar1 + DAT_00f56ccc;
    }
    if (param_1 == 1) {
      return iVar1 + DAT_00f56c7c;
    }
    if (param_1 == 2) {
      return iVar1 + DAT_00f56c74;
    }
    return iVar1 + DAT_00f56c6c;
  }
  if (param_1 == 0) {
    return iVar1 + DAT_00f57284;
  }
  if (param_1 == 1) {
    return iVar1 + DAT_00f5728c;
  }
  if (param_1 == 2) {
    return iVar1 + DAT_00f57294;
  }
  return iVar1 + DAT_00f5729c;
}
