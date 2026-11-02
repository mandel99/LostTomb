#include "restore_types.h"
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd632 (*(undefined4 *)0x00bcd632u)
#define DAT_00bcd634 (*(undefined4 *)0x00bcd634u)
#define DAT_00bcd636 (*(undefined4 *)0x00bcd636u)
#define DAT_00c8ad38 (*(undefined4 *)0x00c8ad38u)
extern void FUN_004b3250();
extern undefined4 FUN_0051cba0();

void FUN_004b33d0(int param_1)

{
  char cVar1;
  int iVar2;
  
  iVar2 = param_1 * 0x184;
  if (*(short *)(&DAT_00bcd632 + iVar2) < 1) {
    cVar1 = FUN_0051cba0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                         (int)*(short *)(&DAT_00bcd61c + iVar2),
                         (int)*(short *)(&DAT_00bcd61e + iVar2));
    (&DAT_00bcd609)[iVar2] = cVar1;
    if (cVar1 != '\b') {
      (&DAT_00bcd609)[iVar2] = 10;
      return;
    }
  }
  else {
    if (*(short *)(&DAT_00bcd636 + iVar2) <= *(short *)(&DAT_00bcd634 + iVar2)) {
      FUN_004b3250(param_1);
      (&DAT_00bcd609)[iVar2] = 8;
      return;
    }
    (&DAT_00bcd609)[iVar2] =
         *(undefined1 *)
          ((int)&DAT_00c8ad38 +
          *(short *)(&DAT_00bcd632 + iVar2) * 500 + (int)*(short *)(&DAT_00bcd634 + iVar2));
  }
  return;
}
