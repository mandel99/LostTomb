#include "restore_types.h"
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd744 (*(undefined4 *)0x00bcd744u)
#define DAT_00bcd746 (*(undefined4 *)0x00bcd746u)
#define DAT_00bcd754 (*(undefined4 *)0x00bcd754u)
extern bool FUN_004a4300();
extern void FUN_004a5680();
extern void FUN_004b3250();

void FUN_004a6890(int param_1,undefined4 *param_2)

{
  char cVar1;
  int iVar2;
  
  iVar2 = param_1 * 0x184;
  if (((&DAT_00bcd60c)[param_1 * 0xc2] == *(short *)(&DAT_00bcd61c + iVar2)) &&
     ((&DAT_00bcd60e)[param_1 * 0xc2] == *(short *)(&DAT_00bcd61e + iVar2))) {
    (&DAT_00bcd609)[iVar2] = 8;
  }
  else {
    FUN_004a5680(param_1,0);
    *param_2 = 1;
  }
  cVar1 = (&DAT_00bcd609)[iVar2];
  if (cVar1 == '\t') {
    FUN_004b3250(param_1);
    return;
  }
  if (cVar1 == '\n') {
    FUN_004b3250(param_1);
    return;
  }
  if (cVar1 == '\b') {
    *param_2 = 0;
    cVar1 = (&DAT_00bcd630)[iVar2];
    if (((cVar1 != '\x14') && (cVar1 != '\n')) && (cVar1 != '\r')) {
      cVar1 = FUN_004a4300(param_1);
      if (cVar1 != '\0') {
        (&DAT_00bcd630)[iVar2] = 0x17;
        *(undefined4 *)(&DAT_00bcd754 + iVar2) = 0x16;
        *(undefined2 *)(&DAT_00bcd744 + iVar2) = (&DAT_00bcd60c)[param_1 * 0xc2];
        *(undefined2 *)(&DAT_00bcd746 + iVar2) = (&DAT_00bcd60e)[param_1 * 0xc2];
      }
    }
  }
  return;
}
