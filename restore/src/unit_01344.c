#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a0c (*(undefined4 *)0x00799a0cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
extern undefined4 FUN_00480060();

undefined4 FUN_00497a30(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = (short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x108;
  cVar1 = (&DAT_00799a0c)[iVar5];
  iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar5 + 2);
  iVar2 = (int)*(short *)(&DAT_007999e0 + iVar5);
  if (cVar1 == '\0') {
    iVar2 = iVar2 + -8;
  }
  else if (cVar1 == '\x02') {
    iVar4 = iVar4 + 8;
  }
  else if (cVar1 == '\x04') {
    iVar2 = iVar2 + 8;
  }
  else if (cVar1 == '\x06') {
    iVar4 = iVar4 + -8;
  }
  if (iVar4 < 0) {
    iVar4 = 0;
  }
  if (iVar2 < 0) {
    iVar2 = 0;
  }
  if (DAT_00bca2d0 <= iVar4) {
    iVar4 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= iVar2) {
    iVar2 = DAT_00bca2d4 + -1;
  }
  iVar4 = FUN_00480060(iVar4,iVar2,1,6);
  if (iVar4 == 0) {
    iVar4 = 0;
    while( true ) {
      cVar1 = (&DAT_00799a0c)[iVar5];
      (&DAT_00799a0c)[iVar5] = cVar1 + 2U;
      if (6 < (byte)(cVar1 + 2U)) {
        (&DAT_00799a0c)[iVar5] = 0;
      }
      iVar2 = (int)*(short *)((int)&DAT_007999dc + iVar5 + 2);
      iVar3 = (int)*(short *)(&DAT_007999e0 + iVar5);
      if (cVar1 == '\0') {
        iVar3 = iVar3 + -3;
      }
      else if (cVar1 == '\x02') {
        iVar2 = iVar2 + 3;
      }
      else if (cVar1 == '\x04') {
        iVar3 = iVar3 + 3;
      }
      else if (cVar1 == '\x06') {
        iVar2 = iVar2 + -3;
      }
      if (iVar2 < 0) {
        iVar2 = 0;
      }
      if (iVar3 < 0) {
        iVar3 = 0;
      }
      if (DAT_00bca2d0 <= iVar2) {
        iVar2 = DAT_00bca2d0 + -1;
      }
      if (DAT_00bca2d4 <= iVar3) {
        iVar3 = DAT_00bca2d4 + -1;
      }
      iVar2 = FUN_00480060(iVar2,iVar3,1,6);
      if (iVar2 != 0) break;
      iVar4 = iVar4 + 1;
      if (3 < iVar4) {
        return 0;
      }
    }
  }
  else {
    cVar1 = (&DAT_00799a0c)[iVar5];
    (&DAT_00799a0c)[iVar5] = cVar1 + 2U;
    if (6 < (byte)(cVar1 + 2U)) {
      (&DAT_00799a0c)[iVar5] = 0;
    }
  }
  return 1;
}
