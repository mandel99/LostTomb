#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799b34 (*(undefined4 *)0x00799b34u)
#define DAT_00e38ec3 (*(char *)0x00e38ec3u)
extern void FUN_004f1310();

undefined4 FUN_004f5340(void)

{
  int iVar1;
  byte *pbVar2;
  int iVar3;
  uint uVar4;
  undefined1 local_70 [52];
  int local_3c;
  
  uVar4 = 0xffffffff;
  iVar3 = 0;
  if (DAT_00e38ec3 != '\0') {
    iVar1 = 1;
    if (0 < DAT_0078d790) {
      pbVar2 = &DAT_00799b34;
      do {
        if (((pbVar2[-0x54] == 1) && (*(short *)(pbVar2 + -0x44) == 0xb7)) &&
           ((int)uVar4 < (int)(uint)*pbVar2)) {
          iVar3 = iVar1;
          uVar4 = (uint)*pbVar2;
        }
        iVar1 = iVar1 + 1;
        pbVar2 = pbVar2 + 0x108;
      } while (iVar1 <= DAT_0078d790);
      if (-1 < (int)uVar4) {
        FUN_004f1310(iVar3,local_70);
        if (local_3c < 0x32) {
          return 2;
        }
        if (local_3c < 0x4b) {
          return 1;
        }
      }
    }
  }
  return 0;
}
