#include "restore_types.h"
#define DAT_0056f348 (*(undefined4 *)0x0056f348u)
#define DAT_0056f349 (*(undefined4 *)0x0056f349u)
#define DAT_0056f34a (*(undefined4 *)0x0056f34au)
#define DAT_0056f34b (*(undefined4 *)0x0056f34bu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a39 (*(undefined4 *)0x00799a39u)

void FUN_00401be0(int param_1)

{
  int iVar1;
  int iVar2;
  int *in_ECX;
  int *piVar3;
  int iVar4;
  
  iVar4 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  iVar1 = *(int *)(&DAT_00799a2c + iVar4 * 0x84);
  piVar3 = (int *)(&DAT_00799a2c + iVar4 * 0x84);
  if (iVar1 < 0x38) {
    if ((&DAT_0056f349)[iVar1 * 4] == '\0') {
      iVar2 = (byte)(&DAT_0056f34b)[iVar1 * 4] - 1;
      (&DAT_00799a39)[iVar2 % 3 + (iVar2 / 3 + iVar4 * 0x58) * 3] =
           (&DAT_0056f348)[iVar1 * 4] == '\x03';
    }
    else {
      iVar2 = iVar4;
      switch((&DAT_0056f34a)[iVar1 * 4]) {
      case 1: ;
        iVar2 = iVar4 + 0xe;
        break;
      case 2: ;
        iVar2 = iVar4 + 0xb;
        break;
      case 3: ;
        iVar2 = iVar4 + 0xd;
        break;
      case 4: ;
        iVar2 = iVar4 + 0xf;
        break;
      case 5: ;
        iVar2 = iVar4 + 0xc;
      }
      (&DAT_00799a30)[iVar2 * 0x42] = (uint)(byte)(&DAT_0056f34b)[iVar1 * 4];
    }
    if (iVar4 < (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) + 0x10) {
      do {
        *piVar3 = iVar1 + 1;
        (**(code **)(*in_ECX + 0x18))(iVar4);
        iVar4 = iVar4 + 1;
        piVar3 = piVar3 + 0x42;
      } while (iVar4 < (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) + 0x10);
    }
  }
  return;
}
