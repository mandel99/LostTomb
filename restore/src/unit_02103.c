#include "restore_types.h"
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd718 (*(undefined4 *)0x00bcd718u)
extern uint FUN_004eea10();
extern undefined4 FUN_004ef100();
extern void FUN_004ef200();

void FUN_004ef3a0(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  
  iVar2 = FUN_004ef100(param_1);
  if ((iVar2 != 0) &&
     (((short)(&DAT_00799a3c)[param_1 * 0x84] == param_2 ||
      (cVar1 = FUN_004eea10(param_1), cVar1 == '\0')))) {
    param_2 = param_2 * 0x184;
    *(short *)(&DAT_00bcd62e + param_2) = *(short *)(&DAT_00bcd62e + param_2) + 1;
    if (0x1d < *(short *)(&DAT_00bcd62e + param_2)) {
      FUN_004ef200(param_1);
      (&DAT_00bcd630)[param_2] = 0xb;
      (&DAT_00799a3c)[param_1 * 0x84] = 0;
      *(undefined2 *)(&DAT_00bcd718 + param_2) = 0;
    }
    return;
  }
  (&DAT_00bcd630)[param_2 * 0x184] = 0xb;
  *(undefined2 *)(&DAT_00bcd718 + param_2 * 0x184) = 0;
  return;
}
