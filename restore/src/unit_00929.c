#include "restore_types.h"
#define DAT_007999d9 (*(undefined4 *)0x007999d9u)
#define DAT_00799a04 (*(undefined4 *)0x00799a04u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_00488050();
extern void FUN_004b1df0();
extern int FUN_004b4800();

void FUN_00467220(int param_1)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  char cVar4;
  int iVar5;
  
  cVar4 = FUN_00488050(param_1);
  uVar3 = _DAT_00d3b4e4;
  uVar2 = _DAT_00d3b4e0;
  _DAT_00d3b4e0 = uVar2;
  _DAT_00d3b4e4 = uVar3;
  if ((cVar4 == '\0') && (0 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8))) {
    sVar1 = (&DAT_00799a04)[param_1 * 0x84];
    if (sVar1 == 0) {
      iVar5 = FUN_004b4800((&DAT_007999d9)[param_1 * 0x108],5,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
      (&DAT_00799a04)[param_1 * 0x84] = (short)iVar5;
      (&DAT_00bcd630)[iVar5 * 0x184] = 1;
      (&DAT_00bcd652)[iVar5 * 0xc2] = (short)param_1;
      FUN_004b1df0(iVar5);
      _DAT_00d3b4e0 = uVar2;
      _DAT_00d3b4e4 = uVar3;
    }
    else {
      iVar5 = sVar1 * 0x184;
      if ((&DAT_00bcd606)[iVar5] == '\0') {
        (&DAT_00799a04)[param_1 * 0x84] = 0;
      }
      if ((&DAT_00bcd602)[iVar5] != '\x05') {
        (&DAT_00799a04)[param_1 * 0x84] = 0;
      }
      if ((short)(&DAT_00bcd652)[sVar1 * 0xc2] != param_1) {
        (&DAT_00799a04)[param_1 * 0x84] = 0;
        return;
      }
    }
  }
  return;
}
