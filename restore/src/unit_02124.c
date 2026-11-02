#include "restore_types.h"
#define LAB_004f24b0 (*(undefined1 *)0x004f24b0u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f4b38 (*(undefined4 *)0x005f4b38u)
#define DAT_005f5498 (*(undefined4 *)0x005f5498u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00eaac3c (*(undefined *)0x00eaac3cu)
#define DAT_00eaac40 (*(undefined *)0x00eaac40u)
#define DAT_00eaac44 (*(undefined *)0x00eaac44u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)
extern void FUN_00562069();

void FUN_004f25c0(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  if (1 < DAT_00eaaf58) {
    if (DAT_005e4ef4 == 5) {
      iVar4 = 0x10;
      piVar1 = (int *)(&DAT_005f5498 + (uint)*(byte *)(&DAT_00799a2c + DAT_00eaac44 * 0x84) * 0x80);
    }
    else {
      iVar4 = 0x3c;
      piVar1 = (int *)(&DAT_005f4b38 + (uint)*(byte *)(&DAT_00799a2c + DAT_00eaac44 * 0x84) * 0x1e0)
      ;
    }
    iVar3 = 0;
    iVar2 = DAT_00eaac44;
    if (iVar4 != 0) {
      do {
        iVar2 = *piVar1 + (DAT_00eaac44 - (short)(&DAT_007999ea)[DAT_00eaac44 * 0x84]);
        if ((int)((uint)*(byte *)(&DAT_00799a2e + DAT_00eaac44 * 0x84) +
                 (uint)*(byte *)((int)&DAT_00799a2e + DAT_00eaac44 * 0x108 + 1) * 6) <= piVar1[1])
        break;
        iVar3 = iVar3 + 1;
        piVar1 = piVar1 + 2;
      } while (iVar3 < iVar4);
    }
    DAT_00eaac40 = (int)*(short *)((int)&DAT_007999dc + iVar2 * 0x108 + 2);
    DAT_00eaac3c = (int)*(short *)(&DAT_007999e0 + iVar2 * 0x108);
    FUN_00562069(&DAT_00eaac44,DAT_00eaaf58,4,&LAB_004f24b0);
  }
  return;
}
