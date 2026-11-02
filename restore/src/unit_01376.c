#include "restore_types.h"
#define DAT_007090ec (*(undefined4 *)0x007090ecu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
extern undefined4 FUN_004069e0();
extern int FUN_00406a30();
extern undefined4 FUN_004a4340();
extern void FUN_004a45d0();
extern void FUN_004a5da0();
extern void FUN_004b3250();

void FUN_004a4c40(int param_1,undefined4 *param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = param_1 * 0x184;
  FUN_004a5da0(param_1,*(undefined4 *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[iVar3] * 0x38));
  cVar1 = (&DAT_00bcd609)[iVar3];
  if (cVar1 != '\b') {
    if ((cVar1 == '\t') || (cVar1 == '\n')) {
      FUN_004b3250(param_1);
    }
    return;
  }
  *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0;
  *param_2 = 0;
  iVar2 = FUN_004069e0((int)(short)(&DAT_00bcd656)[param_1 * 0xc2]);
  if (0 < iVar2) {
    iVar2 = FUN_00406a30(param_1);
    if (iVar2 != (short)(&DAT_00bcd656)[param_1 * 0xc2]) {
      FUN_004a45d0(param_1);
      return;
    }
  }
  FUN_004a4340(param_1,(int)(short)(&DAT_00bcd656)[param_1 * 0xc2]);
  (&DAT_00bcd630)[iVar3] = 0xf;
  return;
}
