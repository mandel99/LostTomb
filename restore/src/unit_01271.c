#include "restore_types.h"
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00d05720 (*(undefined4 *)0x00d05720u)
#define DAT_00d05724 (*(undefined4 *)0x00d05724u)
#define DAT_00d05ab0 (*(undefined4 *)0x00d05ab0u)
#define DAT_00d05ab4 (*(undefined4 *)0x00d05ab4u)

undefined4 FUN_00487dc0(int param_1,int param_2)

{
  param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  if (((((*(int *)(&DAT_00d05720 + param_1 * 4) == 0) ||
        (*(char *)(*(int *)(&DAT_00d05720 + param_1 * 4) + 0x18) != '\0')) &&
       ((*(int *)(&DAT_00d05724 + param_1 * 4) == 0 ||
        (*(char *)(*(int *)(&DAT_00d05724 + param_1 * 4) + 0x18) != '\0')))) &&
      ((*(int *)(&DAT_00d05ab0 + param_1 * 4) == 0 ||
       (*(char *)(*(int *)(&DAT_00d05ab0 + param_1 * 4) + 0x18) != '\0')))) &&
     ((*(int *)(&DAT_00d05ab4 + param_1 * 4) == 0 ||
      (*(char *)(*(int *)(&DAT_00d05ab4 + param_1 * 4) + 0x18) != '\0')))) {
    return 0;
  }
  return 1;
}
