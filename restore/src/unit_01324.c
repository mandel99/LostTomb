#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)

bool FUN_00495af0(int param_1)

{
  int iVar1;
  
  iVar1 = (int)(short)(&DAT_00bcd656)[param_1 * 0xc2];
  if ((&DAT_007999d8)[iVar1 * 0x108] != '\x01') {
    return true;
  }
  if (((&DAT_007999e8)[iVar1 * 0x84] != 0x4b) && ((&DAT_007999e8)[iVar1 * 0x84] != 0xc0)) {
    return true;
  }
  if ((short)(&DAT_00799a18)[iVar1 * 0x84] < 1) {
    return true;
  }
  return *(short *)(&DAT_00799a8c + iVar1 * 0x108) != param_1;
}
