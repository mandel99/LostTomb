#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
extern undefined1 FUN_0040ec20();
extern undefined1 FUN_00417590();
extern uint FUN_004e0530();
extern undefined1 FUN_004e6b20();
extern uint FUN_004eea10();
extern int FUN_004f29e0();
extern uint FUN_00542110();

undefined4 FUN_004a9730(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = param_1 * 0x184;
  if (*(short *)(&DAT_00bcd660 + iVar3) == 0) goto switchD_004a9791_caseD_b8;
  iVar4 = (int)*(short *)(&DAT_00b4e6fc +
                         *(int *)(&DAT_00bcd620 + *(short *)(&DAT_00bcd660 + iVar3) * 0x184) * 2);
  switch((&DAT_007999e8)[iVar4 * 0x84]) {
  case 0xb7: ;
    cVar1 = FUN_004eea10(iVar4);
    if (cVar1 != '\0') {
      return 1;
    }
    break;
default: ;
    goto switchD_004a9791_caseD_b8;
  case 0xd5: ;
    cVar1 = FUN_00542110(iVar4);
    if (cVar1 != '\0') {
      return 1;
    }
    break;
  case 0xde: ;
    cVar1 = FUN_004e0530(iVar4);
    if (cVar1 != '\0') {
      return 1;
    }
    goto switchD_004a9791_caseD_b8;
  case 0xe1: ;
    cVar1 = FUN_0040ec20(iVar4);
    if (cVar1 != '\0') {
      return 1;
    }
    break;
  case 0xe3: ;
    cVar1 = FUN_00417590(iVar4);
    if (cVar1 != '\0') {
      return 1;
    }
    break;
  case 0xe4: ;
    cVar1 = FUN_004e6b20(iVar4);
    if (cVar1 != '\0') {
      return 1;
    }
  }
  iVar2 = FUN_004f29e0(iVar4,1);
  if (iVar2 != 0) {
    *(short *)(&DAT_00bcd660 + iVar3) = (short)iVar2;
    *(short *)(&DAT_00bcd660 + iVar2 * 0x184) = (short)param_1;
    return 1;
  }
  if (*(short *)(&DAT_00799aa0 + iVar4 * 0x42) == 0) {
switchD_004a9791_caseD_b8: ;
    (&DAT_00bcd630)[iVar3] = 8;
  }
  return 0;
}
