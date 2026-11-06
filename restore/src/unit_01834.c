#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00ea6010 (*(undefined4 *)0x00ea6010u)
#define DAT_00ea6014 (*(undefined4 *)0x00ea6014u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)

void FUN_004d5bc0(void)

{
  short sVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = 0;
  psVar2 = &DAT_007999e8;
  while (((char)psVar2[-8] != '\x01' || (*psVar2 != 0xd1))) {
    psVar2 = psVar2 + 0x84;
    iVar3 = iVar3 + 1;
    if (0x89b6e7 < (int)psVar2) {
      iVar3 = DAT_00eaac28 * 0x49d8;
      *(undefined4 *)(&DAT_00ea6010 + iVar3) = 0xffffffff;
      *(undefined4 *)(&DAT_00ea6014 + iVar3) = 0xffffffff;
      return;
    }
  }
  iVar4 = DAT_00eaac28 * 0x49d8;
  sVar1 = *(short *)(&DAT_007999e0 + iVar3 * 0x108);
  *(int *)(&DAT_00ea6010 + iVar4) = (int)*(short *)((int)&DAT_007999dc + iVar3 * 0x108 + 2);
  *(int *)(&DAT_00ea6014 + iVar4) = (int)sVar1;
  return;
}
