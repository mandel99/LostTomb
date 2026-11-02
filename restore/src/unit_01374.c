#include "restore_types.h"
#define DAT_007090ec (*(undefined4 *)0x007090ecu)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd754 (*(undefined4 *)0x00bcd754u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_004806c0();
extern bool FUN_004a4300();
extern void FUN_004a5da0();
extern void FUN_004b3250();

void FUN_004a4950(int param_1,undefined4 *param_2)

{
  short sVar1;
  char cVar2;
  int iVar3;
  
  iVar3 = param_1 * 0x184;
  sVar1 = (&DAT_00bcd652)[param_1 * 0xc2];
  if (((&DAT_00bcd60c)[param_1 * 0xc2] == *(short *)(&DAT_00bcd61c + iVar3)) &&
     ((&DAT_00bcd60e)[param_1 * 0xc2] == *(short *)(&DAT_00bcd61e + iVar3))) {
    (&DAT_00bcd609)[iVar3] = 8;
  }
  else {
    FUN_004a5da0(param_1,*(undefined4 *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[iVar3] * 0x38))
    ;
  }
  cVar2 = (&DAT_00bcd609)[iVar3];
  if (cVar2 == '\t') {
    FUN_004b3250(param_1);
  }
  else if (cVar2 == '\n') {
    FUN_004b3250(param_1);
  }
  else if (cVar2 == '\b') {
    *param_2 = 0;
    if (((&DAT_00bcd630)[iVar3] != '\x14') && ((&DAT_00bcd630)[iVar3] != '\v')) {
      cVar2 = FUN_004a4300(param_1);
      if (cVar2 != '\0') {
        (&DAT_00bcd630)[iVar3] = 10;
        *(undefined4 *)(&DAT_00bcd754 + iVar3) = 0x10;
      }
    }
  }
  if (sVar1 != 0) {
    iVar3 = sVar1 * 0x108;
    FUN_004806c0((int)*(short *)((int)&DAT_007999dc + iVar3 + 2),
                 (int)*(short *)(&DAT_007999e0 + iVar3),(&DAT_007999db)[iVar3]);
    if ((_DAT_00d3b4e4 == (short)(&DAT_00bcd60c)[param_1 * 0xc2]) &&
       (_DAT_00d3b4e0 == (short)(&DAT_00bcd60e)[param_1 * 0xc2])) {
      *(short *)(&DAT_00799a8c + iVar3) = (short)param_1;
      return;
    }
    *(undefined2 *)(&DAT_00799a8c + iVar3) = 0;
  }
  return;
}
