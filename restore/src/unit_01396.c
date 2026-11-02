#include "restore_types.h"
#define DAT_007090ec (*(undefined4 *)0x007090ecu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_004a5da0();
extern void FUN_004b3250();

void FUN_004ad450(int param_1,undefined4 *param_2)

{
  char cVar1;
  int iVar2;
  
  iVar2 = param_1 * 0x184;
  FUN_004a5da0(param_1,*(undefined4 *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[iVar2] * 0x38));
  cVar1 = (&DAT_00bcd609)[iVar2];
  if (cVar1 == '\t') {
    FUN_004b3250(param_1);
    return;
  }
  if (cVar1 == '\n') {
    (&DAT_00bcd630)[iVar2] = 4;
    return;
  }
  if (cVar1 == '\b') {
    *param_2 = 0;
  }
  return;
}
