#include "restore_types.h"
#define DAT_00bcd68a (*(undefined4 *)0x00bcd68au)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
extern void FUN_004b3250();

void FUN_004aece0(void)

{
  int iVar1;
  char cVar2;
  int iVar3;
  
  iVar3 = DAT_00d056f8;
  cVar2 = (&DAT_00bcd68a)[DAT_00d056f8 * 0x184];
  iVar1 = DAT_00d056f8 * 0x184;
  (&DAT_00bcd68a)[iVar1] = cVar2 + 1U;
  if (99 < (byte)(cVar2 + 1U)) {
    (&DAT_00bcd68a)[iVar1] = 0;
    FUN_004b3250(iVar3);
  }
  return;
}
