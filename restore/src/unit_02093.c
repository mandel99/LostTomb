#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
extern void FUN_004ee4a0();

void FUN_004ee560(void)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 1;
  do {
    if (((&DAT_007999d8)[iVar2 * 0x108] != '\0') && ((&DAT_007999e8)[iVar2 * 0x84] == 0xb7)) {
      FUN_004ee4a0(iVar2);
      if (DAT_005e4ef4 == 5) {
        iVar1 = *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + iVar2 * 0x84) * 4);
      }
      else {
        iVar1 = *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + iVar2 * 0x84) * 4);
      }
      iVar2 = iVar2 + -1 + iVar1;
    }
    iVar2 = iVar2 + 1;
  } while (iVar2 < 4000);
  return;
}
