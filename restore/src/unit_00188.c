#include "restore_types.h"
#define DAT_00578c6c (*(undefined4 *)0x00578c6cu)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define DAT_00bcb9c8 (*(undefined4 *)0x00bcb9c8u)
#define DAT_00bcb9f6 (*(undefined4 *)0x00bcb9f6u)
#define DAT_00bcb9f8 (*(undefined4 *)0x00bcb9f8u)
#define DAT_00bcba0a (*(undefined4 *)0x00bcba0au)
#define DAT_00bcba31 (*(undefined4 *)0x00bcba31u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
extern void FUN_00430cd0();

void FUN_00416a20(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (DAT_00e38dda == '\x01') {
    iVar2 = 0;
    iVar3 = 6;
    do {
      DAT_00578c6c = DAT_00578c6c + 1;
      if (6 < DAT_00578c6c) {
        DAT_00578c6c = 1;
      }
      iVar1 = DAT_00578c6c * 0x90;
      if (((((&DAT_00bcb9c8)[iVar1] == '\x01') && ((&DAT_00bcba31)[iVar1] == '\0')) &&
          ((&DAT_00bcba0a)[iVar1] != '\0')) && (iVar2 == 0)) {
        iVar2 = DAT_00578c6c;
      }
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    DAT_00578c6c = iVar2;
    if (0 < iVar2) {
      FUN_00430cd0(DAT_00bca2d8 + *(short *)(&DAT_00bcb9f6 + iVar2 * 0x90) +
                   *(short *)(&DAT_00bcb9f8 + iVar2 * 0x90) * 0xe4);
      DAT_00e38dca = 1;
    }
  }
  return;
}
