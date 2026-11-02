#include "restore_types.h"
#define DAT_00799a26 (*(undefined4 *)0x00799a26u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
extern int FUN_0046f1c0();
extern void FUN_004ccc30();
extern void FUN_004d62f0();

void FUN_004fc2d0(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  uVar2 = DAT_00f56f94;
  DAT_00e20778 = 0;
  if ((&DAT_00799a26)[DAT_00eadd6c * 0x108] == '\0') {
    FUN_004d62f0(0x7f,0x17,DAT_00eadd90 + 0x24,param_1,DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
    return;
  }
  uVar3 = FUN_0046f1c0(DAT_00eadd6c);
  FUN_004d62f0(0x7f,0x18,DAT_00eadd90 + 0x24 + DAT_00e20778,param_1,uVar2,0,DAT_00eadd90,
               DAT_00eadd8c);
  iVar1 = DAT_00e20778;
  FUN_004ccc30(8,0,uVar3,DAT_00eadd90 + 0x24 + DAT_00e20778,param_1,uVar2,0);
  DAT_00e20778 = DAT_00e20778 + iVar1;
  FUN_004d62f0(0x7f,0x19,DAT_00eadd90 + 0x24 + DAT_00e20778,param_1,uVar2,0,DAT_00eadd90,
               DAT_00eadd8c);
  return;
}
