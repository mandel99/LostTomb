#include "restore_types.h"
#define DAT_00bcd5e8 (*(undefined4 *)0x00bcd5e8u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60a (*(undefined4 *)0x00bcd60au)
#define DAT_00bcd631 (*(undefined4 *)0x00bcd631u)
#define DAT_00bcd632 (*(undefined4 *)0x00bcd632u)
#define DAT_00bcd634 (*(undefined4 *)0x00bcd634u)
extern void FUN_004b2d20();
extern void FUN_004b33d0();
extern void FUN_004b3530();
extern void FUN_004b3c60();
extern void FUN_004b3f00();
extern undefined4 FUN_004baa10();

void FUN_004b27e0(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  
  if (0 < param_2) {
    iVar2 = param_1 * 0x184;
    do {
      cVar1 = (&DAT_00bcd631)[iVar2];
      param_2 = param_2 + -1;
      (&DAT_00bcd631)[iVar2] = cVar1 + '\x01';
      if ('\x0e' < (char)(cVar1 + '\x01')) {
        FUN_004baa10(param_1,0);
        (&DAT_00bcd631)[iVar2] = 0xf;
        if (*(short *)(&DAT_00bcd632 + iVar2) < 1) {
          FUN_004b2d20(param_1);
        }
        FUN_004b33d0(param_1);
        FUN_004b3530(param_1);
        if ('\a' < (char)(&DAT_00bcd609)[iVar2]) {
          DAT_00bcd5e8 = 0;
          return;
        }
        *(short *)(&DAT_00bcd634 + iVar2) = *(short *)(&DAT_00bcd634 + iVar2) + 1;
        (&DAT_00bcd60a)[iVar2] = (&DAT_00bcd609)[iVar2];
        (&DAT_00bcd631)[iVar2] = 0;
      }
      if ((&DAT_00bcd631)[iVar2] == '\b') {
        FUN_004b3c60(param_1);
      }
      FUN_004b3f00(param_1);
    } while (0 < param_2);
  }
  DAT_00bcd5e8 = 0;
  return;
}
