#include "restore_types.h"
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_0078a620 (*(undefined *)0x0078a620u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f56494 (*(undefined *)0x00f56494u)
extern int FUN_004cde40();
extern int FUN_004cde60();
extern int FUN_0053e590();

void FUN_004484b0(void)

{
  int iVar1;
  
  if (DAT_00785d70 + -0xc < 1) {
    if (DAT_0078a620 == 0) {
      return;
    }
  }
  else {
    iVar1 = FUN_004cde60(DAT_0078a620,DAT_00785d70 + -0xc);
    if (iVar1 < 0x65) goto LAB_004484e1;
  }
  iVar1 = 100;
LAB_004484e1: ;
  iVar1 = FUN_004cde40(0xc4,iVar1);
  FUN_0053e590(DAT_00f56494 + 0x27,DAT_00e38da8 + 0x204,DAT_00e38da4 + 0x50 + iVar1);
  return;
}
