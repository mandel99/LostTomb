#include "restore_types.h"
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00af5d20 (*(undefined4 *)0x00af5d20u)
#define DAT_00b285cd (*(undefined4 *)0x00b285cdu)
#define DAT_00b286b0 (*(undefined4 *)0x00b286b0u)
#define DAT_00b286b1 (*(undefined4 *)0x00b286b1u)
#define DAT_00b74830 (*(undefined4 *)0x00b74830u)
#define DAT_00b74bbc (*(undefined4 *)0x00b74bbcu)
#define DAT_00b74bc0 (*(undefined4 *)0x00b74bc0u)

bool FUN_00475670(int param_1)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = *(uint *)(&DAT_00af5990 + param_1 * 4);
  uVar2 = (uint)((uVar1 & 0x40) != 0);
  if (((&DAT_00b285cd)[param_1] & 0x80) == 0) {
    uVar2 = uVar2 - 1;
  }
  if ((uVar1 & 4) != 0) {
    uVar2 = uVar2 - 1;
  }
  if ((uVar1 & 8) != 0) {
    uVar2 = uVar2 - 1;
  }
  if ((uVar1 & 0x100) != 0) {
    uVar2 = uVar2 - 1;
  }
  if ((&DAT_00b74830)[param_1] != 0) {
    uVar2 = uVar2 - 1;
  }
  uVar1 = *(uint *)(&DAT_00af5d1c + param_1 * 4);
  if ((uVar1 & 0x40) != 0) {
    uVar2 = uVar2 + 1;
  }
  if (((&DAT_00b286b0)[param_1] & 0x80) == 0) {
    uVar2 = uVar2 - 1;
  }
  if ((uVar1 & 4) != 0) {
    uVar2 = uVar2 - 1;
  }
  if ((uVar1 & 8) != 0) {
    uVar2 = uVar2 - 1;
  }
  if ((uVar1 & 0x100) != 0) {
    uVar2 = uVar2 - 1;
  }
  if (*(int *)(&DAT_00b74bbc + param_1 * 4) != 0) {
    uVar2 = uVar2 - 1;
  }
  uVar1 = *(uint *)(&DAT_00af5d20 + param_1 * 4);
  if ((uVar1 & 0x40) != 0) {
    uVar2 = uVar2 + 1;
  }
  if (((&DAT_00b286b1)[param_1] & 0x80) == 0) {
    uVar2 = uVar2 - 1;
  }
  if ((uVar1 & 4) != 0) {
    uVar2 = uVar2 - 1;
  }
  if ((uVar1 & 8) != 0) {
    uVar2 = uVar2 - 1;
  }
  if ((uVar1 & 0x100) != 0) {
    uVar2 = uVar2 - 1;
  }
  if (*(int *)(&DAT_00b74bc0 + param_1 * 4) != 0) {
    uVar2 = uVar2 - 1;
  }
  return uVar2 == 3;
}
