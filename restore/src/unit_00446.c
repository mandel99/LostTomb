#include "restore_types.h"
#define DAT_00579558 (*(undefined *)0x00579558u)
#define DAT_007027be (*(char *)0x007027beu)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a5f (*(undefined4 *)0x00799a5fu)
#define DAT_00799ac0 (*(undefined4 *)0x00799ac0u)
#define DAT_00799ac8 (*(undefined4 *)0x00799ac8u)
#define DAT_00799ad4 (*(undefined4 *)0x00799ad4u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00417ae0();
extern void FUN_004e90f0();

void FUN_0042fb30(int param_1)

{
  int iVar1;
  
  FUN_004e90f0(DAT_00eaac28,-(int)(short)(&DAT_007999f4)[param_1 * 0x84]);
  iVar1 = DAT_00e38ebc;
  (&DAT_007999f4)[param_1 * 0x84] = 0;
  (&DAT_00799a5f)[param_1 * 0x108] = 1;
  (&DAT_00799ac8)[param_1 * 0x42] = 2;
  *(undefined4 *)(&DAT_00799ad4 + param_1 * 0x108) = 0;
  (&DAT_00799ac0)[param_1 * 0x42] = 0;
  if (((iVar1 == 0) && (DAT_00e38ea8 == DAT_00579558)) && (DAT_007027be == '\0')) {
    FUN_00417ae0(6);
  }
  return;
}
