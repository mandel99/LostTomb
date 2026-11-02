#include "restore_types.h"
#define DAT_007090e4 (*(undefined4 *)0x007090e4u)
#define DAT_00bcd5ec (*(undefined4 *)0x00bcd5ecu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd638 (*(undefined4 *)0x00bcd638u)
#define DAT_00bcd678 (*(undefined4 *)0x00bcd678u)
#define DAT_00d0563c (*(undefined *)0x00d0563cu)
extern int FUN_00499b50();

undefined4 FUN_00497cf0(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  
  if (DAT_00d0563c < 1) {
    return 0;
  }
  iVar3 = param_1 * 0x184;
  if ((&DAT_00bcd630)[iVar3] == '\x05') {
    return 0;
  }
  if ((&DAT_00bcd630)[iVar3] == '\x04') {
    return 0;
  }
  if ((&DAT_00bcd638)[iVar3] != '\0') {
    return 0;
  }
  cVar1 = (&DAT_00bcd678)[iVar3];
  (&DAT_00bcd678)[iVar3] = cVar1 + 1U;
  if ((byte)(cVar1 + 1U) < 0x28) {
    return 0;
  }
  (&DAT_00bcd678)[iVar3] = 0;
  iVar2 = FUN_00499b50(param_1,*(undefined4 *)
                                (&DAT_007090e4 + (uint)(byte)(&DAT_00bcd602)[iVar3] * 0x38));
  if (iVar2 < 1) {
    return 0;
  }
  if (10 < DAT_00bcd5ec) {
    return 0;
  }
  (&DAT_00bcd630)[iVar3] = 10;
  *(undefined2 *)(&DAT_00bcd61c + iVar3) = (&DAT_00bcd60c)[param_1 * 0xc2];
  *(undefined2 *)(&DAT_00bcd61e + iVar3) = (&DAT_00bcd60e)[param_1 * 0xc2];
  return 1;
}
