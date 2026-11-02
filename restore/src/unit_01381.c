#include "restore_types.h"
#define DAT_007090ec (*(undefined4 *)0x007090ecu)
#define DAT_007090f4 (*(undefined4 *)0x007090f4u)
#define DAT_007090f8 (*(undefined4 *)0x007090f8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60a (*(undefined4 *)0x00bcd60au)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd628 (*(undefined4 *)0x00bcd628u)
#define DAT_00bcd62a (*(undefined4 *)0x00bcd62au)
#define DAT_00bcd72c (*(undefined4 *)0x00bcd72cu)
#define DAT_00bcd730 (*(undefined4 *)0x00bcd730u)
#define DAT_00bcd74c (*(undefined4 *)0x00bcd74cu)
extern void FUN_004a5da0();
extern int FUN_004cde40();

void FUN_004a5680(int param_1,int param_2)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = param_1 * 0x184;
  if ((((&DAT_00bcd60c)[param_1 * 0xc2] != *(short *)(&DAT_00bcd61c + iVar4)) ||
      ((&DAT_00bcd60e)[param_1 * 0xc2] != *(short *)(&DAT_00bcd61e + iVar4))) ||
     ((&DAT_00bcd609)[iVar4] != '\b')) {
    cVar1 = (&DAT_00bcd74c)[iVar4];
    if (cVar1 == '\0') {
      if (param_2 == 0) {
        iVar4 = *(int *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[iVar4] * 0x38) * 2;
        if (0x11 < iVar4) {
          iVar4 = 0x12;
        }
      }
      else {
        iVar3 = *(int *)(&DAT_00bcd730 + iVar4);
        *(int *)(&DAT_00bcd730 + iVar4) = iVar3 + 1;
        if (*(int *)(&DAT_007090f8 + (uint)(byte)(&DAT_00bcd602)[iVar4] * 0x38) <= iVar3 + 1) {
          *(undefined4 *)(&DAT_00bcd730 + iVar4) = 0;
          (&DAT_00bcd74c)[iVar4] = 2;
          return;
        }
        if ((&DAT_00bcd60a)[iVar4] != (&DAT_00bcd609)[iVar4]) {
          *(undefined4 *)(&DAT_00bcd72c + iVar4) = 0;
        }
        if (((&DAT_00bcd60c)[param_1 * 0xc2] != *(short *)(&DAT_00bcd628 + iVar4)) ||
           ((&DAT_00bcd60e)[param_1 * 0xc2] != *(short *)(&DAT_00bcd62a + iVar4))) {
          *(undefined2 *)(&DAT_00bcd628 + iVar4) = (&DAT_00bcd60c)[param_1 * 0xc2];
          *(undefined2 *)(&DAT_00bcd62a + iVar4) = (&DAT_00bcd60e)[param_1 * 0xc2];
          *(int *)(&DAT_00bcd72c + iVar4) = *(int *)(&DAT_00bcd72c + iVar4) + 1;
        }
        iVar3 = (uint)(byte)(&DAT_00bcd602)[iVar4] * 0x38;
        if (*(int *)(&DAT_007090f4 + iVar3) <= *(int *)(&DAT_00bcd72c + iVar4)) {
          FUN_004a5da0(param_1,*(undefined4 *)(&DAT_007090ec + iVar3));
          return;
        }
        iVar4 = *(int *)(&DAT_007090ec + iVar3) * 2;
        if (0x11 < iVar4) {
          FUN_004a5da0(param_1,0x12);
          return;
        }
      }
      FUN_004a5da0(param_1,iVar4);
    }
    else if (cVar1 == '\x01') {
      uVar2 = FUN_004cde40(*(undefined4 *)
                            (&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[iVar4] * 0x38),0x28);
      FUN_004a5da0(param_1,uVar2);
      iVar3 = *(int *)(&DAT_00bcd730 + iVar4);
      *(int *)(&DAT_00bcd730 + iVar4) = iVar3 + 1;
      if (199 < iVar3 + 1) {
        *(undefined4 *)(&DAT_00bcd730 + iVar4) = 0;
        (&DAT_00bcd74c)[iVar4] = 0;
        return;
      }
    }
    else if ((cVar1 == '\x02') &&
            (iVar3 = *(int *)(&DAT_00bcd730 + iVar4), *(int *)(&DAT_00bcd730 + iVar4) = iVar3 + 1,
            199 < iVar3 + 1)) {
      *(undefined4 *)(&DAT_00bcd730 + iVar4) = 0;
      (&DAT_00bcd74c)[iVar4] = 1;
      return;
    }
  }
  return;
}
