#include "restore_types.h"
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60a (*(undefined4 *)0x00bcd60au)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd610 (*(undefined4 *)0x00bcd610u)
#define DAT_00bcd612 (*(undefined4 *)0x00bcd612u)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd631 (*(undefined4 *)0x00bcd631u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
extern void FUN_004b3c60();
extern void FUN_004b3f00();
extern undefined4 FUN_0051cba0();

void FUN_004b22e0(int param_1,int param_2,int param_3)

{
  char cVar1;
  int iVar2;
  
  iVar2 = param_1 * 0x184;
  if (((&DAT_00bcd60c)[param_1 * 0xc2] == *(short *)(&DAT_00bcd624 + iVar2)) &&
     ((&DAT_00bcd60e)[param_1 * 0xc2] == *(short *)(&DAT_00bcd626 + iVar2))) {
    (&DAT_00bcd65d)[iVar2] = 1;
  }
  do {
    if (param_3 < 1) {
      return;
    }
    cVar1 = (&DAT_00bcd631)[iVar2];
    param_3 = param_3 + -1;
    (&DAT_00bcd631)[iVar2] = cVar1 + '\x01';
    if ('\x0e' < (char)(cVar1 + '\x01')) {
      (&DAT_00bcd631)[iVar2] = 0xf;
      cVar1 = FUN_0051cba0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                           (int)*(short *)(&DAT_00bcd610 + param_2 * 0x184),
                           (int)*(short *)(&DAT_00bcd612 + param_2 * 0x184));
      (&DAT_00bcd609)[iVar2] = cVar1;
      if ('\a' < cVar1) {
        return;
      }
      (&DAT_00bcd60a)[iVar2] = cVar1;
      (&DAT_00bcd631)[iVar2] = 0;
    }
    FUN_004b3f00(param_1);
    if ((&DAT_00bcd631)[iVar2] == '\b') {
      FUN_004b3c60(param_1);
    }
  } while( true );
}
