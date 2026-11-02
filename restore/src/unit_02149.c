#include "restore_types.h"
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
extern int FUN_004f3690();
extern undefined4 FUN_004f4420();

undefined4 FUN_004f4440(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = param_1 * 0x184;
  iVar2 = DAT_00bca2d8 + (short)(&DAT_00bcd60c)[param_1 * 0xc2] +
          (short)(&DAT_00bcd60e)[param_1 * 0xc2] * 0xe4;
  sVar1 = *(short *)(&DAT_00b4e6fc + iVar2 * 2);
  if (sVar1 == 0) {
    return 0;
  }
  iVar2 = FUN_004f4420(iVar2);
  *(short *)(&DAT_00bcd62e + iVar3) = *(short *)(&DAT_00bcd62e + iVar3) + 1;
  if (iVar2 <= *(short *)(&DAT_00bcd62e + iVar3)) {
    FUN_004f3690((int)*(short *)(&DAT_00bcd61c + iVar3),(int)*(short *)(&DAT_00bcd61e + iVar3));
    (&DAT_00799a3c)[sVar1 * 0x84] = 0;
    return 0;
  }
  return 1;
}
